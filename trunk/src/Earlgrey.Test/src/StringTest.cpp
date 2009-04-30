#include "stdafx.h"
#include "StringHelper.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(StringTest, EqualOperator1) 
		{
			_txstring str1 = TEXT("abcd");
			_txstring str2 = TEXT("abcd");

			ASSERT_TRUE(str1 == str2);
			ASSERT_FALSE(str1 != str2);
		}

		TEST(StringTest, EqualOperator2) 
		{
			_txstring str1 = TEXT("abcd");
			_txstring str2 = TEXT("abcde");

			ASSERT_FALSE(str1 == str2);
			ASSERT_TRUE(str1 != str2);
			ASSERT_TRUE(str1 < str2);
			ASSERT_FALSE(str1 > str2);
		}
	}
}
