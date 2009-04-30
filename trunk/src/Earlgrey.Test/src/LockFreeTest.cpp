#include "stdafx.h"
#include "taskqueue.h"
#include <iostream>

using namespace Earlgrey::Thread::Lockfree;

namespace Earlgrey
{
	namespace Test
	{
		TEST(LockFreeTest, StackTest)
		{
			Stack<int> stack;
			int value = 0;

			EXPECT_FALSE(stack.Pop( value ));

			stack.Push(10);
			stack.Push(9);

			EXPECT_TRUE(stack.Pop( value ));
			EXPECT_TRUE(value == 9);
			EXPECT_TRUE(stack.Pop( value ));
			EXPECT_TRUE(value == 10);
			EXPECT_FALSE(stack.Pop( value ));
		}

		TEST(LockFreeTest, QueueTest)
		{
			Queue<int> queue;
			int value = 0;

			EXPECT_FALSE(queue.Dequeue( value ));

			queue.Enqueue(10);
			queue.Enqueue(1);

			EXPECT_TRUE(queue.Dequeue( value ));
			EXPECT_TRUE(value == 10);
			EXPECT_TRUE(queue.Dequeue( value ));
			EXPECT_TRUE(value == 1);
			EXPECT_FALSE(queue.Dequeue( value ));
		}

		int intval_for_taskq = 0;

		DECLARE_TASK1(SampleTask, int);
		DEFINE_TASK(SampleTask)(int val)
		{
			intval_for_taskq = val;
		}

		DECLARE_TASK0(NoParamTask);
		DEFINE_TASK(NoParamTask)()
		{
			intval_for_taskq = 100;
		}

		TEST(LockFreeTest, TaskQueueTest)
		{
			SimpleTaskQueue taskq;
			taskq.Post(new SampleTask( 10 ));
			EXPECT_TRUE( intval_for_taskq == 10 );
			taskq.Post(new NoParamTask());
			EXPECT_TRUE( intval_for_taskq == 100 );
		}
	}
}