#include "stdafx.h"
#include "StackAllocator.h"

namespace Earlgrey
{
	//namespace Test
	//{
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

				ASSERT_EQ(UINT_MAX, charArray[0]);
				ASSERT_EQ(UINT_MAX, charArray[count - 1]);

				const char expectedChar = std::numeric_limits<char>::min();

				memset(charArray, expectedChar, count);

				ASSERT_EQ(expectedChar, charArray[0]);
				ASSERT_EQ(expectedChar, charArray[count - 1]);
			
				alloc.free(charArray);
			}

			ASSERT_EQ(UINT_MAX, charArray[0]);
		}

		TEST(StackAllocatorTest, AllocTwoObjectsInLIFOWay)
		{
			StackAllocator alloc;				
			char * char1 = static_cast<char*>( 
				alloc.malloc(1)
				);
			*char1 = 'A';

			char * char2 = static_cast<char*>( 
				alloc.malloc(1)
				);
			*char2 = 'B';
			
			alloc.free(char2);
			alloc.free(char1);
		}

		TEST(StackAllocatorTest, AllocTwoObjectsInFIFOWay)
		{
			StackAllocator alloc;				
			char * char1 = static_cast<char*>( 
				alloc.malloc(1)
				);

			char * char2 = static_cast<char*>( 
				alloc.malloc(1)
				);

			alloc.free(char1, 1);
			alloc.free(char2, 1);			
		}
#endif
	//}
}