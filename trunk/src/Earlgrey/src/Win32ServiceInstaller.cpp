#include "stdafx.h"
#include "Win32ServiceInstaller.h"
#include "Win32Service.h"
#include "EarlgreyProcess.h"
#include "tiostream.h"
#include "Log.h"
#include "Registry.h"

namespace Earlgrey
{
	const TCHAR * Win32ServiceInstaller::REGISTRY_SERVICE_ROOT = _T("SYSTEM\\CurrentControlSet\\services");

	Win32ServiceInstaller::Win32ServiceInstaller(Win32Service& service)
		: m_service(service)
		, m_localMachineKey(Registry::LocalMachine())
	{

	}

	void Win32ServiceInstaller::Description(const _txstring& description)
	{
		m_description = description;
	}

	_txstring Win32ServiceInstaller::Description() 
	{
		return m_description;
	}

	const _txstring& Win32ServiceInstaller::Description() const
	{
		return m_description;
	}

	BOOL Win32ServiceInstaller::InstallService() 
	{
		_tstring serviceExecutable;
		try
		{
			serviceExecutable = Process::MainModuleFileName();
			EARLGREY_ASSERT(serviceExecutable.length() > 0);
		}
		catch (std::exception& ex)
		{
			_tcout << TEXT("Couldn't get the executable file name - ")
				<< ex.what()
				<< std::endl
				;
			return FALSE;
		}

		SC_HANDLE scManager = ::OpenSCManager(
			NULL,						// machine (NULL == local)
			SERVICES_ACTIVE_DATABASE,	// database (NULL == SERVICES_ACTIVE_DATABASE == default)
			SC_MANAGER_ALL_ACCESS		// access required
			);

		Earlgrey::handle_t scManagerHandle(scManager, &CloseServiceHandle);

		if(scManager == NULL) {
			const DWORD errCode = GetLastError();
			_tcout << TEXT("OpenSCManager failed - ") << Log::ErrorMessage(errCode) << std::endl;
			return FALSE;
		}

		SC_HANDLE schService = ::CreateService(
			scManager,
			m_service.m_serviceName.c_str(),
			m_service.m_displayName.c_str(),
			m_service.m_dwDesiredAccess,
			m_service.m_dwServiceType,
			m_service.m_dwStartType,
			m_service.m_dwErrorControl,
			serviceExecutable.c_str(),
			m_service.m_pszLoadOrderGroup,
			((m_service.m_dwServiceType == SERVICE_KERNEL_DRIVER ||
			m_service.m_dwServiceType == SERVICE_FILE_SYSTEM_DRIVER) &&
			(m_service.m_dwStartType == SERVICE_BOOT_START ||
			m_service.m_dwStartType == SERVICE_SYSTEM_START)) ?
			&m_service.m_dwTagID : NULL,
			m_service.m_pszDependencies,
			m_service.m_pszStartName,
			m_service.m_pszPassword
			);

		Earlgrey::handle_t schServiceHandle(schService, &CloseServiceHandle);

		if( schService == NULL) {
			const DWORD errCode = GetLastError();
			_tcout << TEXT("CreateService failed - ") << Log::ErrorMessage(errCode) << std::endl;;
			return FALSE;
		}

		// Service description 
		if(m_description.length() > 0)
		{
			TCHAR szKey[MAX_PATH];
			_stprintf_s(szKey, TEXT("%s\\%s"), REGISTRY_SERVICE_ROOT, m_service.ServiceName().c_str());

			HKEY hKey = m_localMachineKey.GetKey(szKey, KEY_WRITE);
			Earlgrey::handle_t regKeyHandle(hKey, &RegCloseKey);
			
			LSTATUS errCode = ::RegSetValueEx(
				hKey,						// handle of key to set value for
				TEXT("Description"),	// address of value to set
				0,							// reserved
				REG_EXPAND_SZ,				// flag for value type
				(CONST BYTE*)m_description.c_str(),		// address of value data
				static_cast<DWORD>(m_description.length() * sizeof(TCHAR))	// size of value data
				);

			if( errCode != ERROR_SUCCESS ) {
				// TODO: Log::ErrorMessage(errCode);
				// throw std::exception("Creating an event source failed!");
				_tcout << "[WARNING] Writing a service description failed!" << std::endl;
			}
		}


		_tcout << m_service.m_displayName << TEXT(" installed.") << std::endl;

		// installation succeeded. Now register the message file
		// TODO: not yet implemented
		/*
		RegisterApplicationLog(
		serviceExecutable.c_str(),		// the path to the application itself
		EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE // supported types
		);

		AddToMessageLog(TEXT("Service installed"), EVENTLOG_INFORMATION_TYPE);
		*/

		return TRUE;
	}

	BOOL Win32ServiceInstaller::RemoveService()
	{
		// SetupConsole();	//!! TCW MOD - have to show the console here for the
		// diagnostic or error reason: orignal class assumed
		// that we were using _main for entry (a console app).
		// This particular usage is a Windows app (no console),
		// so we need to create it. Using SetupConsole with _main
		// is ok - does nothing, since you only get one console.


		//if( OsIsWin95() ) {	//!! TCW MOD - code added to install as Win95 service
		//	HKEY hKey = 0;
		//	LONG lRet = ERROR_SUCCESS;
		//	if( ::RegCreateKey(HKEY_LOCAL_MACHINE, gszWin95ServKey , &hKey) == ERROR_SUCCESS ) {
		//		lRet = ::RegDeleteValue(hKey, m_lpServiceName);
		//		::RegCloseKey(hKey);
		//		bRet=TRUE;
		//	}
		//} 

		// Real NT services go here.
		SC_HANDLE scManager = ::OpenSCManager(
			NULL,						// machine (NULL == local)
			SERVICES_ACTIVE_DATABASE,	// database (NULL == SERVICES_ACTIVE_DATABASE == default)
			SC_MANAGER_ALL_ACCESS		// access required
			);

		Earlgrey::handle_t scManagerHandle(scManager, &CloseServiceHandle);

		if(scManager == NULL) {
			const DWORD errCode = GetLastError();
			_tcout << TEXT("OpenSCManager failed - ") << Log::ErrorMessage(errCode) << std::endl;
			return FALSE;
		}


		SC_HANDLE schService = OpenService(
			scManager,
			m_service.m_serviceName.c_str(),
			SERVICE_ALL_ACCESS
			);

		Earlgrey::handle_t schServiceHandle(schService, &CloseServiceHandle);

		if(scManager == NULL) {
			const DWORD errCode = GetLastError();
			_tcout << TEXT("OpenService failed - ") << Log::ErrorMessage(errCode) << std::endl;
			return FALSE;
		}


		// try to stop the service
		if( ControlService(schService, SERVICE_CONTROL_STOP, &m_service.m_serviceStatus) == FALSE ) 
		{
			const DWORD errCode = GetLastError();
			if(errCode != ERROR_SERVICE_NOT_ACTIVE)
			{
				_tcout << TEXT("Service couldn't be stopped - ") << Log::ErrorMessage(errCode) << std::endl;
				return FALSE;
			}
		}

		_tcout << TEXT("Stopping ") <<  m_service.m_displayName << _T(".") << std::endl;
		Sleep(1000);

		while( QueryServiceStatus(schService, &m_service.m_serviceStatus) ) {
			if( m_service.m_serviceStatus.dwCurrentState == SERVICE_STOP_PENDING ) {
				_tcout << TEXT(".");
				Sleep( 1000 );
			} else
				break;
		}

		if( m_service.m_serviceStatus.dwCurrentState == SERVICE_STOPPED )
			_tcout << std::endl << m_service.m_displayName << TEXT(" stopped.") << std::endl;
		else
			_tcout << std::endl << m_service.m_displayName << TEXT(" failed to stop.") << std::endl;
	

		// now remove the service
		if( DeleteService(schService) == FALSE) 
		{
			const DWORD errCode = GetLastError();
			_tcout << TEXT("DeleteService failed - ") << Log::ErrorMessage(errCode) << std::endl;
			return FALSE;
		}
	
		_tcout << m_service.m_displayName << TEXT(" removed.") << std::endl;

		// TODO: 이벤트 로그를 지우는 게 옳을까?
		// DeregisterApplicationLog();

		return TRUE;
	}
}