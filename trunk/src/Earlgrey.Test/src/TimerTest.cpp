#include "stdafx.h"
#include "Timer.h"

namespace Earlgrey
{
	namespace Test
	{
		static void IncrementStateValue(StatePtr state)
		{
			int* stateValue = reinterpret_cast<int*>(state.get());
			(*stateValue)++;
		}

		static void IncrementStateValueTest(StatePtr timerThreadIdState)
		{
			ThreadIdType * timerThreadIdPtr = reinterpret_cast<ThreadIdType*>(timerThreadIdState.get());

			int * stateValue = new int(7); // �Ʒ� StatePtr �� ����Ʈ ��������
			ASSERT_TRUE(stateValue != NULL);
			StatePtr state(stateValue);

			TimerCallback callback(&IncrementStateValue);

			ASSERT_NE(*timerThreadIdPtr, Thread::CurrentThread()->ThreadId());

			Timer timer(
				callback
				, state
				, TimeSpan::FromMilliseconds(1)
				, TimeSpan::Zero // �� �ѹ��� ����
				, *timerThreadIdPtr
				);

			// TODO: �ϴ� �����Ƽ� Sleep���� ������ Event�� ������ �ؾ� �� ��.
			::Sleep(1000);

			HANDLE waitHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
			ASSERT_TRUE(waitHandle != NULL);
			timer.Close(waitHandle);

			EXPECT_EQ(WAIT_OBJECT_0, ::WaitForSingleObject(waitHandle, 5000));

			ASSERT_EQ(8, *stateValue);

			(*timerThreadIdPtr) = INVALID_THREAD_ID;
		}

		TEST(TimerTest, OneTimeTimer)
		{
			ThreadIdType * stateValue = new ThreadIdType(IO_THREAD_ID_BEGIN); // �Ʒ� StatePtr �� ����Ʈ ��������
			ASSERT_TRUE(stateValue != NULL);
			StatePtr state(stateValue);

			IncrementStateValueTest(state);

			ASSERT_EQ(INVALID_THREAD_ID, *stateValue);
		}

		//! \todo ������ �ߺ��ڵ尡 ����.
		TEST(TimerTest, TwoTimers)
		{
			ThreadIdType * stateValue = new ThreadIdType(WIN_MAIN_THREAD_ID); // �Ʒ� StatePtr �� ����Ʈ ��������
			ASSERT_TRUE(stateValue != NULL);
			StatePtr state(stateValue);

			TimerCallback callback(&IncrementStateValueTest);

			ThreadIdType timerThreadID = IO_THREAD_ID_BEGIN;
			ASSERT_NE(timerThreadID, Thread::CurrentThread()->ThreadId());

			Timer timer(
				callback
				, state
				, TimeSpan::FromMilliseconds(1)
				, TimeSpan::Zero // �� �ѹ��� ����
				, timerThreadID
				);

			// TODO: �ϴ� �����Ƽ� Sleep���� ������ Event�� ������ �ؾ� �� ��.
			::Sleep(1000);

			HANDLE waitHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
			ASSERT_TRUE(waitHandle != NULL);
			timer.Close(waitHandle);

			EXPECT_EQ(WAIT_OBJECT_0, ::WaitForSingleObject(waitHandle, 5000));

			ASSERT_EQ(INVALID_THREAD_ID, *stateValue);
		}
	}
}
