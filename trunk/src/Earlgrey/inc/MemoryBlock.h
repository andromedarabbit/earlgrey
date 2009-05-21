#pragma once
#include "MemoryConfig.h"
#include "MemoryMath.h"
#include "SinglyList.h"

namespace Earlgrey
{
	class DECLSPEC_ALIGN(EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT) MemoryBlock
	{
		friend class SuperMemoryBlock;

	public:
		typedef size_t size_type;

		explicit MemoryBlock(size_type blockSize)
			: m_BlockSize(blockSize)
		{
		}

		inline void * Data()  
		{
			return this + 1;
		}

		inline MemoryBlock* GetNext()
		{
			return reinterpret_cast<MemoryBlock*>(m_Item.Next);
		}

		inline size_type BlockSize() const
		{
			return m_BlockSize;
		}

	private:
		SinglyListEntry	m_Item;
		size_type m_BlockSize;
	};

	class DECLSPEC_ALIGN(EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT) SuperMemoryBlock
	{
	public:
		typedef MemoryBlock::size_type size_type; 
		
		explicit SuperMemoryBlock(size_type chunkSize, size_type blockSize)
			: m_InternalBlockSize( Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT, sizeof(MemoryBlock) + blockSize) )
			, m_ChunkSize( Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_PAGE_ALIGNMENT, chunkSize) )
			// , m_NumberOfBlocks( m_ChunkSize / m_BlockSize )
		{
			EARLGREY_ASSERT(m_ChunkSize >= m_InternalBlockSize);
			EARLGREY_ASSERT(m_ChunkSize > 0 && m_InternalBlockSize > 0);
			EARLGREY_ASSERT(NumberOfBlocks() > 0);
		}

		//! \note 위의 생성자보다 이 생성자가 더 나아보이는데 검토하자.
		/*
		explicit SuperMemoryBlock(size_type blockSize, size_type numberOfBlocks)
			: m_InternalBlockSize( Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT, sizeof(MemoryBlock) + blockSize) )
			, m_ChunkSize( m_InternalBlockSize * numberOfBlocks)
		{
			EARLGREY_ASSERT(m_ChunkSize >= m_InternalBlockSize);
			EARLGREY_ASSERT(m_ChunkSize > 0 && m_InternalBlockSize > 0);
			EARLGREY_ASSERT(NumberOfBlocks() > 0);
		}
		*/

		inline size_type ChunkSize() const
		{
			return m_ChunkSize;
		}

		inline size_type BlockSize() const
		{
			return m_InternalBlockSize - sizeof(MemoryBlock);
		}

		inline size_type NumberOfBlocks() const
		{
			return m_ChunkSize / m_InternalBlockSize;
		}

		inline MemoryBlock * Alloc()
		{
			for(;;)
			{
				// MemoryBlock * node = static_cast<MemoryBlock*>(InterlockedPopEntrySinglyList(&m_BlockHead));
				MemoryBlock * node = reinterpret_cast<MemoryBlock*>(InterlockedPopEntrySinglyList(&m_BlockHead));
				if(node != NULL)
					return node;

				CreateFreeNodes();
			}
		}

		static MemoryBlock * AllocLargeObjectHeap(size_type bytes, DWORD alignment = EARLGREY_DEFAULT_PAGE_ALIGNMENT);

	private:
		inline void * AllocChunk()
		{
			size_type bytes = Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_PAGE_ALIGNMENT, m_ChunkSize);

			void * chunk  = VirtualAlloc(NULL, bytes, MEM_COMMIT | MEM_TOP_DOWN, PAGE_READWRITE); 
			EARLGREY_ASSERT(chunk != NULL);

			//! \note 물리메모리에 붙잡아 놓는다.
			// Applications that need to lock larger numbers of pages must first call the SetProcessWorkingSetSize function 
			// to increase their minimum and maximum working set sizes. The maximum number of pages that a process can lock 
			// is equal to the number of pages in its minimum working set minus a small overhead.
			::VirtualLock(chunk, bytes); 
			return chunk;
		}

		void CreateFreeNodes();

		inline void CreateFreeNode(MemoryBlock * node)
		{
			EARLGREY_ASSERT(node->m_BlockSize == m_InternalBlockSize);
			InterlockedPushEntrySinglyList(&m_BlockHead, &node->m_Item);
		}
		

	private:
		size_type m_ChunkSize;
		size_type m_InternalBlockSize;

		SinglyListHead m_BlockHead;

	};
}
