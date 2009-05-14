#include "stdafx.h"
#include "StackAllocator.h"

namespace Earlgrey
{
	namespace Test
	{
#ifdef _DEBUG
		TEST(StackAllocatorTest, AllocAndFree) 
		{
			char* charArray = NULL;
			{
				const size_t count = 1024;
				const size_t size = count * sizeof(char);

				StackAllocator alloc;				
				charArray = static_cast<char*>( 
					alloc.malloc(size)
					);

				ASSERT_EQ(0, charArray[0]);
				ASSERT_EQ(0, charArray[count - 1]);

				const char expectedChar = std::numeric_limits<char>::max EARLGREY_PREVENT_MACRO_SUBSTITUTION ();

				memset(charArray, expectedChar, count);

				ASSERT_EQ(expectedChar, charArray[0]);
				ASSERT_EQ(expectedChar, charArray[count - 1]);
			
			}

			ASSERT_EQ(0, charArray[0]);
		}
#endif
	}
}