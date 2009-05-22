#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "EarlgreyMath.h"

#include "MemoryBlock.h"


namespace Earlgrey
{

	//! \todo 클래스 이름을 바꾸면 좋겠다.
	//! \note Alignment등도 사용자가 바꾸면 좋을까?
	class GreedyMemoryAllocator : private Uncopyable
	{
	public:
		enum
		{
			DEFAULT_MEMORY_CHUNK_BYTES_PER_ALLOC = EARLGREY_DEFAULT_PAGE_ALIGNMENT * 8
		};

		typedef SuperMemoryBlock::size_type size_type;

		explicit GreedyMemoryAllocator(
			size_type minObjectBytes
			, size_type maxObjectBytes
			, size_type memoryChunkBytesPerAlloc = DEFAULT_MEMORY_CHUNK_BYTES_PER_ALLOC
			)
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

			for(size_type i = 0; i < numberOfSuperMemoryBlocks; i++)
			{
				size_type blockSize = m_MinObjectBytes * (i + 1);
				m_SuperMemoryBlocks[i] = new SuperMemoryBlock(memoryChunkBytesPerAlloc, blockSize);
			}
		}

		//! \todo 블록 해제
		~GreedyMemoryAllocator()
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

		void * Alloc(size_type bytes)
		{
			// 상당한 크기의 메모리 블락을 요구하면 alloc을 따로 받는다.
			// 왜? 단편화 현상을 걱정할 이유가 없으니까!
			if(bytes >= MinLargeObjectBytes()) 
			{
				MemoryBlock * memoryBlock  = SuperMemoryBlock::AllocLargeObjectHeap(bytes, EARLGREY_DEFAULT_PAGE_ALIGNMENT);
				return memoryBlock->Data();
			}

			SuperMemoryBlock * superMemoryBlock = FindSuperMemoryBlock(bytes);
			EARLGREY_ASSERT(superMemoryBlock != NULL);

			return superMemoryBlock->Alloc()->Data();
		}
		

		inline void Free(void * ptr)
		{
			MemoryBlock * memoryBlock = reinterpret_cast<MemoryBlock*>(ptr) - 1;
			size_t blockSize = memoryBlock->BlockSize();
			
			if(blockSize >= MinLargeObjectBytes()) 
			{
				return SuperMemoryBlock::FreeLargeObjectHeap(memoryBlock);
			}

			SuperMemoryBlock * superMemoryBlock = FindSuperMemoryBlock(blockSize);
			EARLGREY_ASSERT(superMemoryBlock != NULL);

			superMemoryBlock->Free(memoryBlock);
		}

	private:
		inline SuperMemoryBlock* FindSuperMemoryBlock(size_type bytes)
		{
			EARLGREY_ASSERT(bytes > 0);

			// size_type index = Math::NewMemoryAligmentPadding(m_MinObjectBytes, bytes) / m_MinObjectBytes;
			size_type index = (bytes - 1) / m_MinObjectBytes;
			return m_SuperMemoryBlocks[index];
		}

	private:
		// unsigned short m_LowerLimitN;
		// unsigned short m_UpperLimitN;
		size_type m_MinObjectBytes;
		size_type m_MaxObjectBytes;

		SuperMemoryBlock ** m_SuperMemoryBlocks;


	};


	typedef 
		Loki::SingletonHolder<GreedyMemoryAllocator, StackMemoryManager::CreateUsingNew, Loki::DefaultLifetime, Loki::SingleThreaded> 
		gGreedyMemoryManager;

}