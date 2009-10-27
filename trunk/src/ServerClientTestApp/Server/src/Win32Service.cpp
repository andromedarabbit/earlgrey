#include "stdafx.h"
#include "Win32Service.h"
#include "SingleAppInstance.h"
#include "Log.h"
#include "EarlgreyProcess.h"

// #include <iostream>
#include "tiostream.h"
#include "RAII.h"

namespace Earlgrey
{

	Win32Service::Win32Service(
							 const TCHAR * serviceName
							 , const TCHAR * displayName
							 )
		: m_serviceName(serviceName)
		, m_displayName(displayName)
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
		if( gSingleInstance::Instance().IsRunning(serviceName) == FALSE )
		{
			throw std::invalid_argument("Win32 service instance already exists!");
		}

		if(displayName == NULL)
			m_displayName = serviceName;
	}

	Win32Service::~Win32Service()
	{

	}

	void Win32Service::Pause()
	{

	}

	void Win32Service::Continue()
	{

	}

	void Win32Service::Shutdown()
	{

	}

	BOOL Win32Service::OnPowerEvent(PowerBroadcastStatus powerStatus)
	{
		return TRUE;
	}



	BOOL Win32Service :: InstallService() {
		// TCHAR szPath[1024];

		// SetupConsole();	//!! TCW MOD - have to show the console here for the
							// diagnostic or error reason: orignal class assumed
							// that we were using _main for entry (a console app).
							// This particular usage is a Windows app (no console),
							// so we need to create it. Using SetupConsole with _main
							// is ok - does nothing, since you only get one console.

		/*
		if( GetModuleFileName( 0, szPath, 1023 ) == 0 ) {
			const DWORD errCode = GetLastError();
			// TCHAR szErr[256];
			// _tprintf(TEXT("Unable to install %s - %s\n"), m_displayName.c_str(), GetLastErrorText(szErr, 256));
			_tprintf(TEXT("Unable to install %s - %s\n"), m_displayName.c_str(), Log::ErrorMessage(errCode));
			return FALSE;
		}
		*/

		_txstring serviceExecutable;
		try
		{
			serviceExecutable = Process::MainModuleFileName();
			EARLGREY_ASSERT(serviceExecutable.length() > 0);
		}
		catch (std::exception& ex)
		{
			_tcerr << TEXT("Couldn't get the executable file name - ")
				<< ex.what()
				<< std::endl
				;
			return FALSE;
		}
		
		/*
		if( OsIsWin95() ) {	//!! TCW MOD - code added to install as Win95 service
			// Create a key for that application and insert values for
			// "EventMessageFile" and "TypesSupported"
			HKEY hKey = 0;
			LONG lRet = ERROR_SUCCESS;
			if( ::RegCreateKey(HKEY_LOCAL_MACHINE, gszWin95ServKey , &hKey) == ERROR_SUCCESS ) {
				lRet =	::RegSetValueEx(
					hKey,				// handle of key to set value for
					m_lpServiceName,	// address of value to set (NAME OF SERVICE)
					0,					// reserved
					REG_EXPAND_SZ,		// flag for value type
					(CONST BYTE*)szPath,// address of value data
					static_cast<DWORD>(_tcslen(szPath) + 1)	// size of value data
					);
				::RegCloseKey(hKey);
				bRet=TRUE;
			}
		} else {
		*/

		SC_HANDLE scManager = ::OpenSCManager(
			NULL,						// machine (NULL == local)
			SERVICES_ACTIVE_DATABASE,	// database (NULL == SERVICES_ACTIVE_DATABASE == default)
			SC_MANAGER_ALL_ACCESS		// access required
			);

		Earlgrey::handle_t scManagerHandle(scManager, &CloseServiceHandle);

		if(scManager == NULL) {
			const DWORD errCode = GetLastError();
			_tcerr << TEXT("OpenSCManager failed - ") << Log::ErrorMessage(errCode) << std::endl;
			return FALSE;
		}
		
		SC_HANDLE schService = ::CreateService(
			scManager,
			m_serviceName.c_str(),
			m_displayName.c_str(),
			m_dwDesiredAccess,
			m_dwServiceType,
			m_dwStartType,
			m_dwErrorControl,
			serviceExecutable.c_str(),
			m_pszLoadOrderGroup,
			((m_dwServiceType == SERVICE_KERNEL_DRIVER ||
			m_dwServiceType == SERVICE_FILE_SYSTEM_DRIVER) &&
			(m_dwStartType == SERVICE_BOOT_START ||
			m_dwStartType == SERVICE_SYSTEM_START)) ?
			&m_dwTagID : 0,
			m_pszDependencies,
			m_pszStartName,
			m_pszPassword
			);

		Earlgrey::handle_t schServiceHandle(schService, &CloseServiceHandle);

		if( schService == NULL) {
			const DWORD errCode = GetLastError();
			_tcerr << TEXT("CreateService failed - ") << Log::ErrorMessage(errCode) << std::endl;;
			return FALSE;
		}

		_tcout << m_displayName << TEXT(" installed.") << std::endl;
		
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
}