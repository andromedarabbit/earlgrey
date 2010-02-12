#include "stdafx.h"
#include "xvector.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(VectorTest, SimplePushBack) 
		{
			auto_vector<char>::Type chars;
			chars.push_back('A');
			chars.push_back('B');
			chars.push_back('C');
			chars.push_back('D');

			ASSERT_EQ(4, chars.size());			
		}

	}
}