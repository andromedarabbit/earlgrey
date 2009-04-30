#include "stdafx.h"
#include "StringHelper.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(StringHelperTest, EndsWith1) 
		{
			_txstring str = TEXT("가나다라마바사");
			_txstring suffix = TEXT("마바사");

			ASSERT_TRUE(StringHelper::EndsWith(str, suffix));
		}

		TEST(StringHelperTest, EndsWith2) 
		{
			_txstring str = TEXT("가나다라마바사");
			_txstring suffix = TEXT("마바사");

			ASSERT_TRUE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE>(str, suffix));
		}

		TEST(StringHelperTest, EndsWith3) 
		{
			_txstring str = TEXT("가나다라마바사");
			_txstring suffix = TEXT("마바");

			ASSERT_FALSE(StringHelper::EndsWith(str, suffix));
		}

		TEST(StringHelperTest, EndsWith4) 
		{
			_txstring str = TEXT("가나다라마바사");
			_txstring suffix = TEXT("마바");

			ASSERT_FALSE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE>(str, suffix));
		}

		TEST(StringHelperTest, EndsWith5) 
		{
			_txstring str = TEXT("ABCDEFG");
			_txstring suffix = TEXT("EFG");

			ASSERT_TRUE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE>(str, suffix));
		}

		TEST(StringHelperTest, EndsWith6) 
		{
			_txstring str = TEXT("ABCDEFG");
			_txstring suffix = TEXT("EF");

			ASSERT_FALSE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE_IGNORECASE>(str, suffix));
		}

		TEST(StringHelperTest, StartsWith1) 
		{
			_txstring str = TEXT("가나다라마바사");
			_txstring suffix = TEXT("가나다");

			ASSERT_TRUE(StringHelper::StartsWith(str, suffix));
		}


	}
}
