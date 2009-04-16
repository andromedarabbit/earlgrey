#include "stdafx.h"
#include "lockfree.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(LockFreeTest, StackTest)
		{
			LockfreeStack32<int> stack;
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
	}
}