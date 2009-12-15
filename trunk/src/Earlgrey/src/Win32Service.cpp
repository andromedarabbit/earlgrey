#include "stdafx.h"
#include "Win32Service.h"
#include "SingleAppInstance.h"
#include "Log.h"

#include <functional>


namespace Earlgrey
{	
	using namespace std;
	using namespace std::tr1;
	using namespace std::tr1::placeholders;

	Win32Service* Win32Service::MAIN_SERVICE = NULL;

	Win32Service::Win32Service(
							 const TCHAR * serviceName
							 , const TCHAR * displayName
							 )
		: m_serviceName(serviceName)
		, m_displayName(displayName)
		, m_eventLog(_T("Application"), serviceName)
		, m_serviceStatusHandle(0)
		, m_checkPoint(0)
		// parameters to the "CreateService()" function:
		, m_dwDesiredAccess(SERVICE_ALL_ACCESS)
		, m_dwServiceType(SERVICE_WIN32_OWN_PROCESS)
		, m_dwStartType(SERVICE_AUTO_START)
		, m_dwErrorControl(SERVICE_ERROR_NORMAL)
		, m_pszLoadOrderGroup(0)
		, m_dwTagID(0)
		, m_pszDependencies(0)
		, m_pszStartName(0)
		, m_pszPassword(0)
	{
		EARLGREY_ASSERT(MAIN_SERVICE == NULL);
		MAIN_SERVICE = this;

		if( gSingleInstance::Instance().RegisterThisApp(serviceName) == FALSE )
		{
			throw std::invalid_argument("Win32 service instance already exists!");
		}

		if(displayName == NULL)
			m_displayName = serviceName;

		// These SERVICE_STATUS members remain as set here.
		m_serviceStatus.dwServiceType = m_dwServiceType;
		m_serviceStatus.dwServiceSpecificExitCode = 0;
	}

	Win32Service::~Win32Service()
	{

	}

	void Win32Service::OnStart(DWORD argc, LPTSTR * argv)
	{
		DBG_UNREFERENCED_PARAMETER(argc);
		DBG_UNREFERENCED_PARAMETER(argv);
	}

	void Win32Service::OnStop()
	{

	}

	void Win32Service::OnPause()
	{

	}

	void Win32Service::OnContinue()
	{

	}

	void Win32Service::OnShutdown()
	{

	}

	BOOL Win32Service::OnCustomCommand(int command)
	{
		DBG_UNREFERENCED_PARAMETER(command);
		throw std::exception("not yet implemented!");
	}

	BOOL Win32Service::OnPowerEvent(PowerBroadcastStatus powerStatus)
	{
		DBG_UNREFERENCED_PARAMETER(powerStatus);
		throw std::exception("not yet implemented!");
	}

	/*
	void WINAPI Win32Service::ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv) 
	{
		// register our service control handler:
		std::tr1::function<void (DWORD)> serviceCtrlBound = std::tr1::bind(
			std::tr1::mem_fn(&Win32Service::ServiceCtrl), std::tr1::ref(*this), _1
			);

		LPHANDLER_FUNCTION serviceCtrlFunc = (LPHANDLER_FUNCTION)serviceCtrlBound.target<void (DWORD)>( );
		EARLGREY_ASSERT(serviceCtrlFunc != NULL);

		m_serviceStatusHandle = ::RegisterServiceCtrlHandler(
			m_serviceName.c_str()
			, serviceCtrlFunc
			);

		if( m_serviceStatusHandle != 0 ) // report the status to the service control manager.
		{			
			if( ReportStatus(SERVICE_START_PENDING) ){
				OnStart( dwArgc, lpszArgv );}
		}

		if( m_serviceStatusHandle != 0 ) // try to report the stopped status to the service control manager.
		{
			ReportStatus(SERVICE_STOPPED);
		}
	}*/

	void WINAPI Win32Service::ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv) 
	{
		EARLGREY_ASSERT(MAIN_SERVICE != NULL);
		MAIN_SERVICE->Start(dwArgc, lpszArgv);
	}

	void Win32Service::Start(DWORD dwArgc, LPTSTR *lpszArgv) 
	{
		// register our service control handler:
		LPHANDLER_FUNCTION_EX serviceCtrlFunc = &Win32Service::ServiceCtrl;
		EARLGREY_ASSERT(serviceCtrlFunc != NULL);

		m_serviceStatusHandle = ::RegisterServiceCtrlHandlerEx(
			m_serviceName.c_str()
			, serviceCtrlFunc
			, this
			);

		if( m_serviceStatusHandle != 0 ) // report the status to the service control manager.
		{			
			if( ReportStatus(SERVICE_START_PENDING) ){
				OnStart( dwArgc, lpszArgv );}
		}

		if( m_serviceStatusHandle != 0 ) // try to report the stopped status to the service control manager.
		{
			ReportStatus(SERVICE_STOPPED);
		}
	}

	DWORD WINAPI Win32Service::ServiceCtrl(
		DWORD    dwControl,
		DWORD    dwEventType,
		LPVOID   lpEventData,
		LPVOID   lpContext
		)
	{
		Win32Service * service = static_cast<Win32Service*>(lpContext);
		EARLGREY_ASSERT(service != NULL);

		return service->ControlHandler(dwControl, dwEventType, lpEventData);
	}

	DWORD Win32Service::ControlHandler(DWORD dwControl, DWORD dwEventType, LPVOID lpEventData)
	{
		DBG_UNREFERENCED_PARAMETER(dwEventType);
		DBG_UNREFERENCED_PARAMETER(lpEventData);

		if(dwControl == SERVICE_CONTROL_STOP)
		{
			m_serviceStatus.dwCurrentState = SERVICE_STOP_PENDING;
			OnStop();
			return NO_ERROR;
		}

		if(dwControl == SERVICE_CONTROL_PAUSE)
		{		
			m_serviceStatus.dwCurrentState = SERVICE_PAUSE_PENDING;
			OnPause();
			return NO_ERROR;
		}

		if(dwControl == SERVICE_CONTROL_CONTINUE)
		{
			m_serviceStatus.dwCurrentState = SERVICE_CONTINUE_PENDING;
			OnContinue();
			return NO_ERROR;
		}

		if(dwControl == SERVICE_CONTROL_SHUTDOWN) 
		{
			OnShutdown();
			return NO_ERROR;
		}


		if(dwControl == SERVICE_CONTROL_INTERROGATE)
		{
			// Update the service status.
			ReportStatus(m_serviceStatus.dwCurrentState);
			return NO_ERROR;
		}

		return ERROR_CALL_NOT_IMPLEMENTED;
	}

	BOOL Win32Service::ReportStatus(DWORD currentState, DWORD waitHint, DWORD errExit)
	{
		// when debugging we don't report to the SCM
		// if( m_bDebug )
		//	return TRUE;

		m_serviceStatus.dwCurrentState = currentState;
		m_serviceStatus.dwWin32ExitCode = NO_ERROR;
		m_serviceStatus.dwWaitHint = waitHint;

		if( currentState == SERVICE_START_PENDING)
			m_serviceStatus.dwControlsAccepted = 0;
		else
			m_serviceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;

		//!! TCW MOD START - added code to support error exiting
		m_serviceStatus.dwServiceSpecificExitCode = errExit;
		if (errExit!=0)
			m_serviceStatus.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
		//!! TCW MOD END - added code to support error exiting

		if( currentState == SERVICE_RUNNING || currentState == SERVICE_STOPPED )
			m_serviceStatus.dwCheckPoint = 0;
		else
			m_serviceStatus.dwCheckPoint = ++m_checkPoint;

		// Report the status of the service to the service control manager.
		BOOL succeeded = ::SetServiceStatus( m_serviceStatusHandle, &m_serviceStatus);
		if (succeeded == FALSE) {
			m_eventLog.WriteEntry(TEXT("SetServiceStatus() failed"), EVENTLOG_ERROR_TYPE);
			return FALSE;
		}

		return TRUE;
	}

	LPSERVICE_MAIN_FUNCTION Win32Service::ServiceMainFunc() const
	{
		return &Win32Service::ServiceMain;
	}

	/************************************************************************/
	/* class methods                                                        */
	/************************************************************************/


	void Win32Service::Run(Win32Service& service)
	{
		// Default implementation creates a single threaded service.
		// Override this method and provide more table entries for
		// a multithreaded service (one entry for each thread).
		LPSERVICE_MAIN_FUNCTION serviceMainFunc = &Win32Service::ServiceMain;
		EARLGREY_ASSERT(serviceMainFunc != NULL);

		SERVICE_TABLE_ENTRY dispatchTable[] =
		{
			{ const_cast<LPTSTR>(service.m_serviceName.c_str()), serviceMainFunc },
			{ 0, 0 }
		};
		
		BOOL bRet = StartServiceCtrlDispatcher(dispatchTable);
		if( ! bRet ) {
			_txstring msg( Log::ErrorMessage(GetLastError()) );
			throw std::exception("Couldn't start the service!");
		}
	}

	/*
	void Win32Service::Run(xvector< shared_ptr<Win32Service> >::Type& services)
	{
		EARLGREY_ASSERT(services.size() > 0);

		SERVICE_TABLE_ENTRY * dispatchTable 
			= auto_ptr<SERVICE_TABLE_ENTRY>( new SERVICE_TABLE_ENTRY[services.size()] ).get();
		EARLGREY_ASSERT(dispatchTable != NULL);
		

		for(size_t i = 0; i < services.size(); i++)
		{
			shared_ptr<Win32Service> service( services[i] );
			EARLGREY_ASSERT(service != NULL);			

			std::tr1::function<void (DWORD, LPWSTR*)> serviceMainBound = std::tr1::bind(
				std::tr1::mem_fn(&Win32Service::ServiceMain), std::tr1::ref(*service), _1, _2
				);

			dispatchTable[i].lpServiceName = const_cast<LPTSTR>(service->m_serviceName.c_str());
			dispatchTable[i].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)serviceMainBound.target<void ( DWORD, LPWSTR * )>( );
			EARLGREY_ASSERT(dispatchTable[i].lpServiceProc != NULL);
		}

		BOOL bRet = StartServiceCtrlDispatcher(dispatchTable);
		if( ! bRet ) {
			_txstring msg( Log::ErrorMessage(GetLastError()) );
			// service.m_eventLog.WriteEntry(msg.c_str(), EVENTLOG_ERROR_TYPE);
			throw std::exception("Couldn't start the service!");
		}
	}*/

	void Win32Service::WriteEventLog(
		const TCHAR * message
		, WORD eventType
		, DWORD eventID
		, WORD category
		)
	{
		m_eventLog.WriteEntry(message, eventType, eventID, category);
	}
}


