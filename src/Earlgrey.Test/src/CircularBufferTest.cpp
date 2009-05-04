#include "stdafx.h"
#include "CircularBuffer.hpp"


namespace Earlgrey
{
	namespace Test
	{

		TEST(AssertTest, False)
		{
			circular_buffer<int> cb(5);

			ASSERT_EQ(0, cb.size());
			ASSERT_EQ(5, cb.capacity());		
			ASSERT_TRUE(cb.empty());
			ASSERT_GT(0, cb.max_size());
		}

	}
}

