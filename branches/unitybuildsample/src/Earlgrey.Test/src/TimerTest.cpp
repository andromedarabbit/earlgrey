#include "stdafx.h"
#include "Timer.h"

namespace Earlgrey
{
	namespace Test
	{
		namespace  
		{
			static void IncrementStateValue(StatePtr state)
			{
				int* stateValue = reinterpret_cast<int*>(state.get());
				(*stateValue)++;
			}

			static void IncrementStateValueTest(ThreadIdType& timerThreadId)
			{
				int * stateValue = new int(7); // �Ʒ� StatePtr �� ����Ʈ ��������
				ASSERT_TRUE(stateValue != NULL);
				StatePtr state(stateValue);

				TimerCallback callback(&IncrementStateValue);

				ASSERT_NE(timerThreadId, Thread::CurrentThread()->ThreadId());

				Timer timer(
					callback
					, state
					, TimeSpan::FromMilliseconds(1)
					, TimeSpan::Zero // �� �ѹ��� ����
					, timerThreadId
					);

				// TODO: �ϴ� �����Ƽ� Sleep���� ������ Event�� ������ �ؾ� �� ��.
				::Sleep(1000);

				HANDLE waitHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
				ASSERT_TRUE(waitHandle != NULL);
				timer.Close(waitHandle);

				EXPECT_EQ(WAIT_OBJECT_0, ::WaitForSingleObject(waitHandle, 5000));

				ASSERT_EQ(8, *stateValue);

				timerThreadId = INVALID_THREAD_ID;
			}
		}		

 		TEST(TimerTest, OneTimeTimer)
 		{
 			// TODO: ���� Earlgrey.Test���� �� ������� Thread loop�� ���� �ʴ´�.
 			// ASSERT_EQ(0, WIN_MAIN_THREAD_ID);
 			for(ThreadIdType i = IO_THREAD_ID_BEGIN; i <= IO_THREAD_ID_END; i++ )
 			{
 				ThreadIdType threadId = i; // �Ʒ� StatePtr �� ����Ʈ ��������
 				
 				IncrementStateValueTest(threadId);
 
 				ASSERT_EQ(INVALID_THREAD_ID, threadId);
 			}
 		}

		//! \todo ������ �ߺ��ڵ尡 ����.
// 		TEST(TimerTest, TwoTimers)
// 		{
// 			ThreadIdType * stateValue = new ThreadIdType(WIN_MAIN_THREAD_ID); // �Ʒ� StatePtr �� ����Ʈ ��������
// 			ASSERT_TRUE(stateValue != NULL);
// 			StatePtr state(stateValue);
// 
// 			TimerCallback callback(&IncrementStateValueTest);
// 
// 			ThreadIdType timerThreadID = IO_THREAD_ID_BEGIN;
// 			ASSERT_NE(timerThreadID, Thread::CurrentThread()->ThreadId());
// 
// 			Timer timer(
// 				callback
// 				, state
// 				, TimeSpan::FromMilliseconds(1)
// 				, TimeSpan::Zero // �� �ѹ��� ����
// 				, timerThreadID
// 				);
// 
// 			// TODO: �ϴ� �����Ƽ� Sleep���� ������ Event�� ������ �ؾ� �� ��.
// 			::Sleep(1000);
// 
// 			HANDLE waitHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
// 			ASSERT_TRUE(waitHandle != NULL);
// 			timer.Close(waitHandle);
// 
// 			EXPECT_EQ(WAIT_OBJECT_0, ::WaitForSingleObject(waitHandle, 5000));
// 
// 			ASSERT_EQ(INVALID_THREAD_ID, *stateValue);
// 		}

		namespace
		{
			struct A
			{

			};
		}
// 
// 		TEST(TimerTest, TwoTimers)
//  		{
//  			ThreadIdType * stateValue = new ThreadIdType(WIN_MAIN_THREAD_ID); // �Ʒ� StatePtr �� ����Ʈ ��������
//  			ASSERT_TRUE(stateValue != NULL);
//  			StatePtr state(stateValue);
//  
//  			TimerCallback callback(&IncrementStateValueTest);
//  
//  			ThreadIdType timerThreadID = IO_THREAD_ID_BEGIN;
//  			ASSERT_NE(timerThreadID, Thread::CurrentThread()->ThreadId());
//  
//  			Timer timer(
//  				callback
//  				, state
//  				, TimeSpan::FromMilliseconds(1)
//  				, TimeSpan::Zero // �� �ѹ��� ����
//  				, timerThreadID
//  				);
//  
//  			// TODO: �ϴ� �����Ƽ� Sleep���� ������ Event�� ������ �ؾ� �� ��.
//  			::Sleep(1000);
//  
//  			HANDLE waitHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
//  			ASSERT_TRUE(waitHandle != NULL);
//  			timer.Close(waitHandle);
//  
//  			EXPECT_EQ(WAIT_OBJECT_0, ::WaitForSingleObject(waitHandle, 5000));
//  
//  			ASSERT_EQ(INVALID_THREAD_ID, *stateValue);
//  		}

 	}
}
