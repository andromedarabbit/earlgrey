#include "stdafx.h"
#include "GreedyAllocator.h"

namespace Earlgrey
{
	namespace Test
	{		
		TEST(GreedyAllocatorTest, Getters)
		{
			GreedyAllocator::size_type minObjectBytes = 16;
			GreedyAllocator::size_type maxObjectBytes = 4096;
			GreedyAllocator::size_type memoryChunkBytesPerAlloc = GreedyAllocator::DEFAULT_MEMORY_CHUNK_BYTES_PER_ALLOC;

			GreedyAllocator manager(minObjectBytes, maxObjectBytes, memoryChunkBytesPerAlloc);
			ASSERT_EQ(minObjectBytes, manager.MinObjectBytes());
			ASSERT_EQ(maxObjectBytes, manager.MaxObjectBytes());
			ASSERT_EQ(maxObjectBytes + 1, manager.MinLargeObjectBytes());
		}

#ifdef _DEBUG
		TEST(GreedyAllocatorTest, AllocAndFree)
		{
			GreedyAllocator::size_type minObjectBytes = 16;
			GreedyAllocator::size_type maxObjectBytes = 4096;
			GreedyAllocator::size_type memoryChunkBytesPerAlloc = GreedyAllocator::DEFAULT_MEMORY_CHUNK_BYTES_PER_ALLOC;

			GreedyAllocator manager(minObjectBytes, maxObjectBytes, memoryChunkBytesPerAlloc);


			size_t t = sizeof(MemoryBlock);
			DBG_UNREFERENCED_LOCAL_VARIABLE(t);

			GreedyAllocator::size_type bytes = 64;
			void * ptr = manager.Alloc(bytes);

			char * charArray = reinterpret_cast<char*>(ptr);

			for(BYTE i = 0; i < bytes / sizeof(char); i++)
			{
				char value = charArray[i];
				ASSERT_EQ(UINT_MAX, value);
			}


			// Free한 뒤에도 실제 메모리를 운영체제에 반납하진 않는다.
			// 그래서 다음과 같은 코드가 유효하다.
			manager.Free(ptr);

			for(BYTE i = 0; i < bytes / sizeof(char); i++)
			{
				char value = charArray[i];
				ASSERT_EQ(std::numeric_limits<UINT>::min(), value);
			}
		}
#endif
	}
}