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


		TEST(StackAllocatorTest, CopyConstructor)
		{
			StackAllocator alloc1;
			char * char1 = static_cast<char*>( 
				alloc1.malloc(1)
				);

			{
				StackAllocator alloc2(alloc1);
				char * char2 = static_cast<char*>( 
					alloc2.malloc(128)
					);
				alloc2.free(char2, 128);
			}
						
			alloc1.free(char1, 1);
			
		}


		TEST(StackAllocatorTest, CopyConstructor1)
		{
			StackAllocator alloc1;
			char * char1 = static_cast<char*>( 
				alloc1.malloc(1)
				);

			{
				StackAllocator alloc2(alloc1);
				char * char2 = static_cast<char*>( 
					alloc1.malloc(128)
					);
				alloc1.free(char2, 128);
			}
			alloc1.free(char1, 1);
		}


		TEST(StackAllocatorTest, CopyConstructor2)
		{
			StackAllocator alloc1;
			char * char1 = static_cast<char*>( 
				alloc1.malloc(1)
				);

			{
				StackAllocator alloc2(alloc1);
				
				{
					StackAllocator alloc3(alloc2);

					char * char2 = static_cast<char*>( 
						alloc3.malloc(128)
						);
					alloc3.free(char2, 128);
				}
			}


			alloc1.free(char1, 1);

		}


		TEST(StackAllocatorTest, CopyConstructor3)
		{
			StackAllocator alloc1;
			char * char1 = static_cast<char*>( 
				alloc1.malloc(1)
				);

			char *  char2 = NULL;
			{
				StackAllocator alloc2(alloc1);
				char2 = static_cast<char*>( 
					alloc1.malloc(128)
					);
				alloc1.free(char2, 128);
			}

			// alloc1.free(char2, 128);
			alloc1.free(char1, 1);

		}
#endif
	//}
}