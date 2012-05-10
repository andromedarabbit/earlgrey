#include "stdafx.h"
#include "EventLog.h"
#include "EarlgreyProcess.h"
#include "Log.h"
#include "RAII.h"

#include "Registry.h"
#include "StringComparison.hpp"

namespace Earlgrey
{

	
	const WCHAR * EventLog::REGISTRY_EVENTLOG_ROOT = L"SYSTEM\\CurrentControlSet\\services\\eventlog";

	EventLog::EventLog(const WCHAR * const logName, const WCHAR * const source)
		: m_logName(logName)
		, m_source(source)
		// , m_eventSource(NULL)
		, m_eventSource(
			::RegisterEventSourceW(0, m_source.c_str())
			, &DeregisterEventSource
			)
		, m_userSID(NULL) // TODO: get a real user sid
		, m_localMachineKey(Registry::LocalMachine())
	{		
		if( m_eventSource == NULL ) {
			// TODO GetLastError
			throw std::exception("Registering an event source failed!");
		}


		/////////////////////////////////////////////////////////////////////////
		// Providing a SID (security identifier) was contributed by Victor
		// Vogelpoel (VictorV@Telic.nl).
		// The code from Victor was slightly modified.

		// Get security information of current user
		BYTE	security_identifier_buffer[ 4096 ];
		DWORD	dwSizeSecurityIdBuffer = sizeof( security_identifier_buffer );

		WCHAR sUserName[ 256 ];
		DWORD dwSizeUserName  =  255;

		WCHAR sDomainName[ 256 ];
		DWORD dwSizeDomainName = 255;

		SID_NAME_USE sidTypeSecurityId;

		::ZeroMemory( sUserName, sizeof( sUserName ) );
		::ZeroMemory( sDomainName, sizeof( sDomainName ) );
		::ZeroMemory( security_identifier_buffer, dwSizeSecurityIdBuffer );

		::GetUserNameW( sUserName, &dwSizeUserName );

		if( ::LookupAccountNameW(
			0,
			sUserName,
			&security_identifier_buffer,
			&dwSizeSecurityIdBuffer,
			sDomainName,
			&dwSizeDomainName,
			&sidTypeSecurityId
			)) {
				if( ::IsValidSid( PSID(security_identifier_buffer) ) ) {
					DWORD dwSidLen = ::GetLengthSid(PSID(security_identifier_buffer));
					m_userSID = PSID(new BYTE [dwSidLen]);
					::CopySid(dwSidLen, m_userSID, security_identifier_buffer);
					_ASSERTE(::EqualSid(m_userSID, security_identifier_buffer));
				}
		}
	}

	EventLog::~EventLog()
	{
		delete [] LPBYTE(m_userSID);
	}


	//! \todo adds more overloading methods (ReportEvent function has a few parameters)
	void EventLog::WriteEntry(
		const WCHAR * message
		, WORD eventType
		, DWORD eventID
		, WORD category
		)
	{
		// Use event logging to log the error.
		// LPCTSTR lpszMessage = ;

		BOOL successful = ::ReportEventW(
			m_eventSource.get(),	// handle of event source
			eventType,		// event type
			category,			// event category
			eventID,		// event ID
			m_userSID,		// current user's SID
			1,				// strings in lpszStrings
			0,				// no bytes of raw data
			&message,	// array of error strings
			0				// no raw data
			);

		// ::DeregisterEventSource(hEventSource);
		if(!successful)
		{
			// TODO  GetLastError
			throw std::exception("reporting an event failed!");
		}

	}


	void EventLog::Clear()
	{
		 if(ClearEventLog(m_eventSource.get(), NULL) == FALSE)
		 {
			// TODO: GetLastError
			// TODO: failed when it is already empty
			throw std::exception("Clearing the event log failed!");
		 }
	}

	DWORD EventLog::NumberOfEventLogRecords() const
	{
		DWORD numberOfRecords = 0;
		if(::GetNumberOfEventLogRecords(m_eventSource.get(), &numberOfRecords) == FALSE)
		{
			// TODO: GetLastError
			throw std::exception("Getting the number of event log records failed!");
		}
		return numberOfRecords;
	}

	/************************************************************************/
	/* Static methods                                                       */
	/************************************************************************/

	BOOL EventLog::Exists(const WCHAR * const logName)
	{
		WCHAR szKey[MAX_PATH];
		swprintf_s(szKey, L"%s\\%s", REGISTRY_EVENTLOG_ROOT, logName);

		RegistryKey localMachineKey(Registry::LocalMachine());
		return localMachineKey.KeyExists(szKey);
	}

	BOOL EventLog::SourceExists(const WCHAR * const source)
	{
		RegistryKey localMachineKey(Registry::LocalMachine());

		xvector<xwstring>::Type logNames;
		if(localMachineKey.GetSubKeyNames(REGISTRY_EVENTLOG_ROOT, logNames) == FALSE)
			return FALSE;



		StringComparison<STRCMP_CURRENT_CULTURE_IGNORECASE> comparer;
		WCHAR logPath[MAX_PATH];
		
		xvector<xwstring>::Type::const_iterator it = logNames.begin();
		for( ; it != logNames.end(); it++)
		{
			const xwstring& logName = *it;
			swprintf_s(logPath, L"%s\\%s", REGISTRY_EVENTLOG_ROOT, logName.c_str());

			xvector<xwstring>::Type sourceNames;
			if(localMachineKey.GetSubKeyNames(logPath, sourceNames) == FALSE)
				continue;

			xvector<xwstring>::Type::const_iterator it2 = sourceNames.begin();
			for( ; it2 != sourceNames.end(); it2++)
			{
				const xwstring& sourceName = *it2;
				if( comparer.Equals( sourceName.c_str(), source ) )
				{
					return TRUE;
				}
			}
		}
		return FALSE;
	}
	
	void EventLog::CreateEventSource(const WCHAR * const source)
	{
		CreateEventSource(source, L"Application");
	}

	//! \todo 내부에서 하는 레지스트리 조작 작업을 클래스 Registry에 위임한다.
	void EventLog::CreateEventSource(const WCHAR * const source, const WCHAR * const logName)
	{
		_tstring mainModuleFileName = Process::MainModuleFileName();

		WCHAR szKey[MAX_PATH];
		swprintf_s(szKey, L"%s\\%s\\%s", REGISTRY_EVENTLOG_ROOT, logName, source);
		
		HKEY hKey = 0;
		handle_t regKeyHandle(hKey, &RegCloseKey);
		// LONG lRet = ERROR_SUCCESS;

		// Create a key for that application and insert values for
		// "EventMessageFile" and "TypesSupported"
		LONG errCode = ::RegCreateKeyW(HKEY_LOCAL_MACHINE, szKey, &hKey);
		if( errCode != ERROR_SUCCESS ) {
			// TODO: Log::ErrorMessage(errCode);
			// throw std::exception("Creating an event source failed!");
			const CHAR * const errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}
	
		errCode = ::RegSetValueExW(
			hKey,						// handle of key to set value for
			L"EventMessageFile",	// address of value to set
			0,							// reserved
			REG_EXPAND_SZ,				// flag for value type
			(CONST BYTE*)mainModuleFileName.c_str(),		// address of value data
			static_cast<DWORD>(mainModuleFileName.length() * sizeof(WCHAR))	// size of value data
			);

		if( errCode != ERROR_SUCCESS ) {
			// TODO: Log::ErrorMessage(errCode);
			// throw std::exception("Creating an event source failed!");
			const CHAR * const errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}

		// Set the supported types flags.
		errCode = ::RegSetValueExW(
			hKey,					// handle of key to set value for
			L"TypesSupported",	// address of value to set
			0,						// reserved
			REG_DWORD,				// flag for value type
			(CONST BYTE*)&SUPPORTED_EVENT_TYPES,	// address of value data
			sizeof(DWORD)			// size of value data
			);
		// ::RegCloseKey(hKey);

		if( errCode != ERROR_SUCCESS ) {
			// TODO: Log::ErrorMessage(errCode);
			// throw std::exception("Creating an event source failed!");
			const CHAR * const errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}
	}

	void EventLog::Delete(const WCHAR * const logName)
	{
		WCHAR szKey[MAX_PATH];
		swprintf_s(szKey, L"%s\\%s", REGISTRY_EVENTLOG_ROOT, logName);

		RegistryKey localMachineKey(Registry::LocalMachine());
		if(localMachineKey.DeleteSubKeyTree(szKey) == FALSE)
		{
			throw std::exception("Deleting an event log failed!");
		}
	}

	void EventLog::DeleteEventSource(const WCHAR * const source)
	{
		EARLGREY_ASSERT(source != NULL);

		xwstring eventSourcePath(
			FindEventSource(source)
			);

		if(eventSourcePath.length() == 0)
			throw std::exception("The event source couldn't be found!");

		RegistryKey localMachineKey(Registry::LocalMachine());
		if(localMachineKey.DeleteSubKeyTree(eventSourcePath.c_str()) == FALSE)
		{
			throw std::exception("Deleting an event log failed!");
		}
	}

	xwstring EventLog::FindEventSource(const WCHAR * const source)
	{
		RegistryKey localMachineKey(Registry::LocalMachine());

		xvector<xwstring>::Type logNames;
		if(localMachineKey.GetSubKeyNames(REGISTRY_EVENTLOG_ROOT, logNames) == FALSE)
			return FALSE;


		StringComparison<STRCMP_CURRENT_CULTURE_IGNORECASE> comparer;
		WCHAR logPath[MAX_PATH];

		xvector<xwstring>::Type::const_iterator it = logNames.begin();
		for( ; it != logNames.end(); it++)
		{
			const xwstring& logName = *it;
			swprintf_s(logPath, L"%s\\%s", REGISTRY_EVENTLOG_ROOT, logName.c_str());

			xvector<xwstring>::Type sourceNames;
			if(localMachineKey.GetSubKeyNames(logPath, sourceNames) == FALSE)
				continue;

			xvector<xwstring>::Type::const_iterator it2 = sourceNames.begin();
			for( ; it2 != sourceNames.end(); it2++)
			{
				const xwstring& sourceName = *it2;
				if( comparer.Equals( sourceName.c_str(), source ) )
				{
					swprintf_s(logPath, L"%s\\%s", logPath, sourceName.c_str());
					return xwstring(logPath);
				}
			}
		}
		return xwstring();
	}

}