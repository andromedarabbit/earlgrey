#include "stdafx.h"
#include "ThreadLocal.hpp"

namespace Earlgrey
{
	namespace Test
	{
		TEST(ThreadLocalTest, IntTest)
		{
			const int integerValue = 5;

			ThreadLocalValue<int> tlsInteger(integerValue);
			int tlsValue1 = tlsInteger.Get();

			ASSERT_EQ(integerValue, tlsValue1);

			int* tlsValue2 = &tlsValue1;
			// int* tlsValue2 = tlsInteger;;

			ASSERT_EQ(integerValue, *tlsValue2);
		}

		TEST(ThreadLocalTest, IntPtrTest)
		{
			int integerValue = 3;

			ThreadLocalValue<int*> tlsInteger(&integerValue);
			int* tlsValue1 = tlsInteger.Get();

			ASSERT_EQ(integerValue, *tlsValue1);

			int tlsValue2 = *tlsInteger;
			
			ASSERT_EQ(integerValue, tlsValue2);
		}
	}
}