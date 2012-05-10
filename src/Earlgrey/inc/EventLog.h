#pragma once
#include "Uncopyable.h"
#include "txstring.h"
#include "RAII.h"
#include "RegistryKey.h"

namespace Earlgrey
{
	//! \todo supports remote event log?
	//! \ref http://www.codeproject.com/KB/system/CEventLog.aspx
	//! \ref http://www.codeguru.com/cpp/w-p/system/ntservices/article.php/c2809/
	//! \ref http://www.codeguru.com/cpp/w-p/system/ntservices/article.php/c2847/
	//! \ref http://msdn.microsoft.com/en-us/library/ms810429.aspx
	//! \ref http://www.muukka.net/programming/service/
	class EventLog : private Uncopyable
	{
		static const DWORD SUPPORTED_EVENT_TYPES = 
			EVENTLOG_SUCCESS 
			| EVENTLOG_ERROR_TYPE
			| EVENTLOG_WARNING_TYPE
			| EVENTLOG_INFORMATION_TYPE
			| EVENTLOG_AUDIT_SUCCESS
			| EVENTLOG_AUDIT_FAILURE
			;

		static const WCHAR * REGISTRY_EVENTLOG_ROOT; 

	public: // instance methods
		explicit EventLog(const WCHAR * const logName, const WCHAR * const source);
		~EventLog();

		inline const WCHAR * const Log() const
		{
			return m_logName.c_str();
		}

		inline const WCHAR * const Source() const
		{
			return m_source.c_str();
		}

		void WriteEntry(
			const WCHAR * message
			, WORD eventType = EVENTLOG_INFORMATION_TYPE
			, DWORD eventID = 0
			, WORD category = 0 
			);

		void Clear();

		DWORD NumberOfEventLogRecords() const;

	public: // static methods
		static BOOL Exists(const WCHAR * const logName);
		static BOOL SourceExists(const WCHAR * const source);
		static void CreateEventSource(const WCHAR * const source);
		static void CreateEventSource(const WCHAR * const source, const WCHAR * const logName);

		static void Delete(const WCHAR * const logName);
		static void DeleteEventSource(const WCHAR * const source);

		static xwstring FindEventSource(const WCHAR * const source);

	private:
		xwstring m_logName;
		xwstring m_source;

		// Earlgrey::handle_t m_eventSource;
		Earlgrey::handle_t m_eventSource;
		PSID m_userSID;			// the current user's security identifier
		RegistryKey m_localMachineKey;
	};
}