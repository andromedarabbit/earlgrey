#include "stdafx.h"
#include "EventLog.h"


namespace Earlgrey
{
	namespace Test
	{
		class EventLogTest : public ::testing::Test {
		protected:
			// You can define per-test set-up and tear-down logic as usual.
			virtual void SetUp() { 
				DeleteEventSource();
			}
			virtual void TearDown() {
				DeleteEventSource();				
			}

			static const WCHAR * source;
			static const WCHAR * log;
		private:
			void DeleteEventSource()
			{
				try
				{
					EventLog eventLog(log, source);
					eventLog.Clear();

					EventLog::DeleteEventSource( source );
				}
				catch(...)
				{
				}
			}
		};

		const WCHAR * EventLogTest::source = L"EarlgreyTestSource";
		const WCHAR * EventLogTest::log = L"EarlgreyTestLog";

		TEST_F(EventLogTest, Exists)
		{
			ASSERT_TRUE2( EventLog::Exists(L"Application") );		
			ASSERT_TRUE2( EventLog::Exists(L"application") );	 // Case insensitive
			ASSERT_TRUE2( EventLog::Exists(L"System") );
			ASSERT_FALSE2( EventLog::Exists(L"None") );			
		}

		TEST_F(EventLogTest, SourceExists)
		{
			ASSERT_TRUE2( EventLog::SourceExists(L"COM") );
			ASSERT_TRUE2( EventLog::SourceExists(L"com") ); // Case insensitive
			ASSERT_FALSE2( EventLog::SourceExists(L"NONE-NONE") );
		}


		TEST_F(EventLogTest, CreateEventSource)
		{
			ASSERT_FALSE2( EventLog::SourceExists(source) );
			EventLog::CreateEventSource(source, L"Application");
			ASSERT_TRUE2( EventLog::SourceExists(source) );
			EventLog::DeleteEventSource(source);
			ASSERT_FALSE2( EventLog::SourceExists(source) );
		}


		TEST_F(EventLogTest, WriteEntryAndClear)
		{
			if(EventLog::SourceExists(source) == FALSE) {
				EventLog::CreateEventSource(source, log);
			}
			ASSERT_TRUE2( EventLog::SourceExists(source) );

			EventLog eventLog(log, source);
			ASSERT_EQ(0, eventLog.NumberOfEventLogRecords());

			eventLog.WriteEntry(L"Test", EVENTLOG_INFORMATION_TYPE);
			ASSERT_EQ(1, eventLog.NumberOfEventLogRecords());
			
			eventLog.Clear();
			ASSERT_EQ(0, eventLog.NumberOfEventLogRecords());

			EventLog::DeleteEventSource(source);
			ASSERT_FALSE2( EventLog::SourceExists(source) );
		}
	}
}
