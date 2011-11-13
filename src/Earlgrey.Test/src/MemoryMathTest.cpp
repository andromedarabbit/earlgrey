#include "stdafx.h"
#include "MemoryMath.h"


namespace Earlgrey
{
	namespace Math
	{
		namespace Test
		{		
			TEST(MemoryMathTest, NewMemoryAligmentOffset0)
			{
				size_t align = 64;
				size_t offset = 0;

				size_t newOffset = Earlgrey::Math::NewMemoryAlignmentOffset(align, offset);
				ASSERT_EQ(0, newOffset);
			}

			TEST(MemoryMathTest, NewMemoryAligmentOffset2)
			{
				size_t align = 64;
				size_t offset = 64;

				size_t newOffset = Earlgrey::Math::NewMemoryAlignmentOffset(align, offset);
				ASSERT_EQ(64, newOffset);
			}

			TEST(MemoryMathTest, NewMemoryAligmentOffset3)
			{
				size_t align = 64;
				size_t offset = 65;

				size_t newOffset = Earlgrey::Math::NewMemoryAlignmentOffset(align, offset);
				ASSERT_EQ(128, newOffset);
			}
		}
	}
}