#include "stdafx.h"
#include "taskqueue.h"
#include <iostream>


namespace Earlgrey
{
	namespace Test
	{
		TEST(LockFreeTest, StackTest)
		{
			Lockfree::Stack<int> stack;
			int value = 0;

			EXPECT_FALSE(stack.pop( value ));

			stack.push(10);
			stack.push(9);

			EXPECT_TRUE(stack.pop( value ));
			EXPECT_TRUE(value == 9);
			EXPECT_TRUE(stack.pop( value ));
			EXPECT_TRUE(value == 10);
			EXPECT_FALSE(stack.pop( value ));
		}

		TEST(LockFreeTest, QueueTest)
		{
			Lockfree::Queue<int> queue;
			int value = 0;

			EXPECT_FALSE(queue.dequeue( value ));

			queue.enqueue(10);
			queue.enqueue(1);

			EXPECT_TRUE(queue.dequeue( value ));
			EXPECT_TRUE(value == 10);
			EXPECT_TRUE(queue.dequeue( value ));
			EXPECT_TRUE(value == 1);
			EXPECT_FALSE(queue.dequeue( value ));
		}

		int intval_for_taskq = 0;

		DECLARE_TASK1(SampleTask, int);
		DEFINE_TASK1(SampleTask, int, val)
		{
			intval_for_taskq = val;
		}

		DECLARE_TASK0(NoParamTask);
		DEFINE_TASK0(NoParamTask)
		{
			intval_for_taskq = 100;
		}

		TEST(LockFreeTest, TaskQueueTest)
		{
			Lockfree::TaskQueue taskq;
			taskq.execute(new SampleTask( 10 ));
			EXPECT_TRUE( intval_for_taskq == 10 );
			taskq.execute(new NoParamTask());
			EXPECT_TRUE( intval_for_taskq == 100 );
		}
	}
}