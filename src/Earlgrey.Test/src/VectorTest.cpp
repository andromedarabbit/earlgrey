#include "stdafx.h"
#include "xvector.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(VectorTest, SimplePushBack1) 
		{
			auto_vector<char>::Type chars;
			chars.reserve(128);

			chars.push_back('A');
			chars.push_back('B');

			ASSERT_EQ(2, chars.size());			
		}

		TEST(VectorTest, SimplePushBack2) 
		{
			auto_vector<char>::Type chars;
			chars.push_back('A');
			chars.push_back('B');

			ASSERT_EQ(2, chars.size());			
		}

	}
}