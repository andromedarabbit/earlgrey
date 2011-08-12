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
				int * stateValue = new int(7); // 아래 StatePtr 이 스마트 포인터임
				ASSERT_TRUE(stateValue != NULL);
				StatePtr state(stateValue);

				TimerCallback callback(&IncrementStateValue);

				ASSERT_NE(timerThreadId, Thread::CurrentThread()->ThreadId());

				Timer timer(
					callback
					, state
					, TimeSpan::FromMilliseconds(1)
					, TimeSpan::Zero // 단 한번만 실행
					, timerThreadId
					);

				// TODO: 일단 귀찮아서 Sleep으로 했지만 Event를 쓰던가 해야 할 듯.
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
 			// TODO: 현재 Earlgrey.Test에서 주 스레드는 Thread loop를 돌지 않는다.
 			// ASSERT_EQ(0, WIN_MAIN_THREAD_ID);
 			for(ThreadIdType i = IO_THREAD_ID_BEGIN; i <= IO_THREAD_ID_END; i++ )
 			{
 				ThreadIdType threadId = i; // 아래 StatePtr 이 스마트 포인터임
 				
 				IncrementStateValueTest(threadId);
 
 				ASSERT_EQ(INVALID_THREAD_ID, threadId);
 			}
 		}

		//! \todo 여전히 중복코드가 많다.
// 		TEST(TimerTest, TwoTimers)
// 		{
// 			ThreadIdType * stateValue = new ThreadIdType(WIN_MAIN_THREAD_ID); // 아래 StatePtr 이 스마트 포인터임
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
// 				, TimeSpan::Zero // 단 한번만 실행
// 				, timerThreadID
// 				);
// 
// 			// TODO: 일단 귀찮아서 Sleep으로 했지만 Event를 쓰던가 해야 할 듯.
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
//  			ThreadIdType * stateValue = new ThreadIdType(WIN_MAIN_THREAD_ID); // 아래 StatePtr 이 스마트 포인터임
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
//  				, TimeSpan::Zero // 단 한번만 실행
//  				, timerThreadID
//  				);
//  
//  			// TODO: 일단 귀찮아서 Sleep으로 했지만 Event를 쓰던가 해야 할 듯.
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
