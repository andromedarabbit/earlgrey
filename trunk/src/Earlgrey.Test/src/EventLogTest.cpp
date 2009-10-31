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

			static const TCHAR * source;
		private:
			void DeleteEventSource()
			{
				try
				{
					EventLog::DeleteEventSource( source );
				}
				catch(...)
				{
				}
			}
		};

		const TCHAR * EventLogTest::source = _T("EarlgreyTest");

		TEST_F(EventLogTest, Exists)
		{
			ASSERT_TRUE( EventLog::Exists(_T("Application")) );		
			ASSERT_TRUE( EventLog::Exists(_T("application")) );	 // Case insensitive
			ASSERT_TRUE( EventLog::Exists(_T("System")) );
			ASSERT_FALSE( EventLog::Exists(_T("None")) );			
		}

		TEST_F(EventLogTest, SourceExists)
		{
			ASSERT_TRUE( EventLog::SourceExists(_T("COM")) );
			ASSERT_TRUE( EventLog::SourceExists(_T("com")) ); // Case insensitive
			ASSERT_FALSE( EventLog::SourceExists(_T("NONE-NONE")) );
		}


		TEST_F(EventLogTest, CreateEventSource)
		{
			// const TCHAR * source = _T("EarlgreyTest");
			ASSERT_FALSE( EventLog::SourceExists(source) );
			EventLog::CreateEventSource(source, _T("Application"));
			ASSERT_TRUE( EventLog::SourceExists(source) );
			EventLog::DeleteEventSource(source);
			ASSERT_FALSE( EventLog::SourceExists(source) );

		}


		TEST_F(EventLogTest, WriteEntryAndClear)
		{
			const TCHAR * source = _T("EarlgreyTestSource");	
			const TCHAR * const log = _T("EarlgreyTestLog");

			ASSERT_FALSE(EventLog::SourceExists(source));
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
