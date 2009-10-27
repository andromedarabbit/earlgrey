#include "stdafx.h"
#include "EventLog.h"


namespace Earlgrey
{
	namespace Test
	{
		TEST(EventLogTest, Exists)
		{
			ASSERT_TRUE( EventLog::Exists(_T("Application")) );		
			ASSERT_TRUE( EventLog::Exists(_T("application")) );	 // Case insensitive
			ASSERT_TRUE( EventLog::Exists(_T("System")) );
			ASSERT_FALSE( EventLog::Exists(_T("None")) );			
		}

		TEST(EventLogTest, SourceExists)
		{
			ASSERT_TRUE( EventLog::SourceExists(_T("COM")) );
			ASSERT_TRUE( EventLog::SourceExists(_T("com")) ); // Case insensitive
			ASSERT_FALSE( EventLog::SourceExists(_T("NONE-NONE")) );
		}


		TEST(EventLogTest, CreateEventSource)
		{
			const TCHAR * source = _T("EarlgreyTest");
			ASSERT_FALSE( EventLog::SourceExists(source) );
			EventLog::CreateEventSource(source, _T("Application"));
			ASSERT_TRUE( EventLog::SourceExists(source) );
			EventLog::DeleteEventSource(source);
			ASSERT_FALSE( EventLog::SourceExists(source) );
		}


		TEST(EventLogTest, WriteEntryAndClear)
		{
			const TCHAR * source = _T("EarlgreyTestSource");	
			const TCHAR * const log = _T("EarlgreyTestLog");

			if(EventLog::SourceExists(source) == FALSE) {
				EventLog::CreateEventSource(source, log);
			}
			ASSERT_TRUE( EventLog::SourceExists(source) );

			EventLog eventLog(log, source);
			ASSERT_EQ(0, eventLog.NumberOfEventLogRecords());

			eventLog.WriteEntry(_T("Test"), EVENTLOG_INFORMATION_TYPE);
			ASSERT_EQ(1, eventLog.NumberOfEventLogRecords());
			
			eventLog.Clear();
			ASSERT_EQ(0, eventLog.NumberOfEventLogRecords());

			EventLog::DeleteEventSource(source);
			ASSERT_FALSE( EventLog::SourceExists(source) );
		}
	}
}
