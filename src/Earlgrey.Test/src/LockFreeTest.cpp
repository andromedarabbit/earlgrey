#include "stdafx.h"
#include "lockfree.h"

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
	}
}