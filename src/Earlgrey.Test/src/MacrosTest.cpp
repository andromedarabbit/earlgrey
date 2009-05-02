#include "stdafx.h"
#include "Macros.h"


#undef max

namespace Earlgrey
{
	namespace Test
	{		
		TEST(MacrosTest, Binary1)
		{
			const int nibble = BINARY1(0101);  
			ASSERT_EQ(5, nibble);
		}

		TEST(MacrosTest, Binary2)
		{
			const BYTE byte = BINARY2(1010,0101);
			ASSERT_EQ(165, byte);
		}

		TEST(MacrosTest, Binary8)
		{
			const DWORD dword1 = BINARY8(0000,0000,0000,0000,0000,0000,1010,0101);
			ASSERT_EQ(165, dword1);

			const DWORD dword2 = BINARY8(1111,1111,1111,1111,1111,1111,1111,1111);
			ASSERT_EQ(std::numeric_limits<DWORD>::max(), dword2);
		}
	}
}