#include "stdafx.h"
#include "lexical_cast.hpp"

#include "tstring.h"
#include "txstring.h"

namespace Earlgrey
{
	namespace Test
	{
		using namespace std;

		TEST(StringConversionTest, ToString1)
		{
			const double sourceNumber = 12.345;
			_txstring str = String::lexical_cast<_txstring>(sourceNumber);

			_txstring expected = TEXT("12.345");
			ASSERT_STREQ(expected.c_str(), str.c_str());
		}

		TEST(StringConversionTest, ToString2)
		{
			const double sourceNumber = 12.3456789;
			_tstring str = String::lexical_cast<_tstring>(sourceNumber);

			// Now has a precision limit 
			// refer to http://gpgstudy.com/forum/viewtopic.php?topic=24400
			_tstring expected = TEXT("12.345");
			ASSERT_STRNE(TEXT("12.345"), str.c_str());
		}
	

		TEST(StringConversionTest, ToDouble)
		{
			double d = String::lexical_cast<double>("12.333");
			ASSERT_DOUBLE_EQ(12.333, d);
		}

		TEST(StringConversionTest, ToFloat)
		{
			float d = String::lexical_cast<float>("98.765");
			ASSERT_FLOAT_EQ(98.765f, d);
		}
	}
}
