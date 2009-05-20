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
			: m_BlockSize( Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT, sizeof(MemoryBlock) + blockSize) )
			, m_ChunkSize( Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_PAGE_ALIGNMENT, chunkSize) )
			// , m_NumberOfBlocks( m_ChunkSize / m_BlockSize )
		{
			EARLGREY_ASSERT(chunkSize >= blockSize);
			EARLGREY_ASSERT(chunkSize > 0 && blockSize > 0);
			EARLGREY_ASSERT( Math::IsPowerOf2(m_BlockSize) );

			// 강제할 필요는 없지만, 되도록 매개변수를 던지기 전에 바이트 수를 생각해보면 좋겠다.
			EARLGREY_ASSERT( m_ChunkSize % m_BlockSize == 0 ); 
		}

		inline size_type ChunkSize() const
		{
			return m_ChunkSize;
		}

		inline size_type BlockSize() const
		{
			return m_BlockSize;
		}

		inline size_type NumberOfBlocks() const
		{
			return m_ChunkSize / m_BlockSize;
		}

		inline MemoryBlock * Data()
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

	private:
		void * AllocChunk()
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

		void CreateFreeNodes()
		{
			BYTE * newChunk = static_cast<BYTE*>( AllocChunk() );

			const size_type count = NumberOfBlocks();
			EARLGREY_ASSERT(count > 0);

			for (size_type i = 0; i < count; i++)
			{
				MemoryBlock * node = reinterpret_cast<MemoryBlock*>(newChunk + m_BlockSize * i);
				EARLGREY_ASSERT(node != NULL);
				
				new( reinterpret_cast<void*>(node)) MemoryBlock(m_BlockSize); // 생성자 호출
				CreateFreeNode(node);
			}
		}

		inline void CreateFreeNode(MemoryBlock * node)
		{
			EARLGREY_ASSERT(node->m_BlockSize == m_BlockSize);
			InterlockedPushEntrySinglyList(&m_BlockHead, &node->m_Item);
		}
		

	private:
		size_type m_ChunkSize;
		size_type m_BlockSize;

		SinglyListHead m_BlockHead;

	};
}
