#include "stdafx.h"
#include "LockfreeStack.hpp"
#include "LockfreeQueue.hpp"
#include <iostream>

using namespace Earlgrey::Algorithm::Lockfree;

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
	}
}
