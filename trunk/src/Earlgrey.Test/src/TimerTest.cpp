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

		TEST(TimerTest, OneTimeTimer)
		{
			int * stateValue = new int(7); // 아래 StatePtr 이 스마트 포인터임
			ASSERT_TRUE(stateValue != NULL);
			StatePtr state(stateValue);

			TimerCallback callback(&IncrementStateValue);

			ASSERT_NE(IO_THREAD_ID_BEGIN, Thread::CurrentThread()->ThreadId());

			Timer timer(
				callback
				, state
				, TimeSpan::FromMilliseconds(1)
				, TimeSpan::Zero // 단 한번만 실행
				, IO_THREAD_ID_BEGIN
				);
			
			// TODO: 일단 귀찮아서 Sleep으로 했지만 Event를 쓰던가 해야 할 듯.
			::Sleep(1000);

			HANDLE waitHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
			ASSERT_TRUE(waitHandle != NULL);
			timer.Close(waitHandle);

			EXPECT_EQ(WAIT_OBJECT_0, ::WaitForSingleObject(waitHandle, 5000));

			ASSERT_EQ(8, *stateValue);
		}
	}
}
