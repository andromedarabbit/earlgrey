#include "stdafx.h"
#include "StaticAssert.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(StaticAssert, BoostLibraryExamples) 
		{
			// http://www.boost.org/doc/libs/1_38_0/doc/html/boost_staticassert.html
			EARLGREY_STATIC_ASSERT(std::numeric_limits<int>::digits < 32);
			EARLGREY_STATIC_ASSERT(WCHAR_MIN >= 0);
		}
	}
}