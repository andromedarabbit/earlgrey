#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "EarlgreyMath.h"

#include "MemoryBlock.h"


namespace Earlgrey
{

	//! \todo 클래스 이름을 바꾸면 좋겠다.
	class MemoryManager : private Uncopyable
	{
	public:
		/*
		enum 
		{
			DEFAULT_NUMBER_OF_MEMORY_BLOCKS_PER_AN_ALLOCATION = 1024;
		};
		*/
		enum
		{
			DEFAULT_MEMORY_CHUNK_BYTES_PER_ALLOC = EARLGREY_DEFAULT_PAGE_ALIGNMENT * 8
		};

		typedef SuperMemoryBlock::size_type size_type;

		explicit MemoryManager(size_type minObjectBytes, size_type maxObjectBytes, size_t memoryChunkBytesPerAlloc = DEFAULT_MEMORY_CHUNK_BYTES_PER_ALLOC)
			: m_MinObjectBytes(minObjectBytes)
			, m_MaxObjectBytes(maxObjectBytes)
			, m_SuperMemoryBlocks(NULL)
		{
			EARLGREY_ASSERT(m_MinObjectBytes % EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT == 0);
			EARLGREY_ASSERT(m_MaxObjectBytes % EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT == 0);
			EARLGREY_ASSERT(m_MinObjectBytes <= m_MaxObjectBytes);
			EARLGREY_ASSERT(EARLGREY_DEFAULT_PAGE_ALIGNMENT <= MinLargeObjectBytes());

			const size_type numberOfSuperMemoryBlocks 
				= (MaxObjectBytes() - m_MinObjectBytes) / EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT + 1;

			m_SuperMemoryBlocks = new SuperMemoryBlock*[numberOfSuperMemoryBlocks];
			EARLGREY_ASSERT(m_SuperMemoryBlocks != NULL);

			for(size_type i = 1; i <= numberOfSuperMemoryBlocks; i++)
			{
				size_t blockSize = m_MinObjectBytes * i;
				m_SuperMemoryBlocks[i] = new SuperMemoryBlock(memoryChunkBytesPerAlloc, blockSize);
			}
		}

		//! \todo 블록 해제
		~MemoryManager()
		{
			
		}

		inline size_type MinObjectBytes() const
		{
			return m_MinObjectBytes;
		}

		inline size_type MaxObjectBytes() const
		{
			return m_MaxObjectBytes;
		}

		inline size_type MinLargeObjectBytes() const
		{
			return m_MaxObjectBytes + 1;
		}

		void * Malloc(size_type bytes)
		{
			// 상당한 크기의 메모리 블락을 요구하면 alloc을 따로 받는다.
			// 왜? 단편화 현상을 걱정할 이유가 없으니까!
			if(bytes >= MinLargeObjectBytes()) 
			{
				MemoryBlock * memoryBlock  = SuperMemoryBlock::AllocLargeObjectHeap(bytes, EARLGREY_DEFAULT_PAGE_ALIGNMENT);
				return memoryBlock->Data();
			}

			return NULL;
		}
		

		inline void Free(void * ptr)
		{
			MemoryBlock *memoryBlock = reinterpret_cast<MemoryBlock*>(ptr) - 1;
			size_t blockSize = memoryBlock->BlockSize();
			EARLGREY_ASSERT(blockSize > 0);

			if(blockSize >= MinLargeObjectBytes()) 
			{
				_aligned_free(ptr);
			}
		}


	private:
		// unsigned short m_LowerLimitN;
		// unsigned short m_UpperLimitN;
		size_type m_MinObjectBytes;
		size_type m_MaxObjectBytes;

		SuperMemoryBlock ** m_SuperMemoryBlocks;


	};


}