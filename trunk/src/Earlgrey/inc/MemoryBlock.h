#pragma once
#include "MemoryConfig.h"
#include "MemoryMath.h"
#include "SinglyList.h"

#include "Macros.h"
#include "Lock.h"

#include <limits>

// #undef min
// #undef max 

namespace Earlgrey
{
	// Memory block without SLIST things. This will be used in Thread safe allocators.
	class DECLSPEC_ALIGN(EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT) SimpleMemoryBlock
	{
	public:
		typedef size_t size_type;

		inline void * Data()  
		{
			return this + 1;
		}

		SimpleMemoryBlock* Next;
		size_type BlockSize;
	};

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
			: m_InternalBlockSize( 
				Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT
				, sizeof(MemoryBlock) + blockSize) 
				)
			, m_ChunkSize( 
				Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_PAGE_ALIGNMENT, chunkSize) 
				)
			, m_BlockHead(
				)
		{
			EARLGREY_ASSERT(m_ChunkSize >= m_InternalBlockSize);
			EARLGREY_ASSERT(m_ChunkSize > 0 && m_InternalBlockSize > 0);
			EARLGREY_ASSERT(NumberOfBlocks() > 0);
		}
		
		explicit SuperMemoryBlock(size_type blockSize, unsigned short numberOfBlocks)
			: m_InternalBlockSize( 
				Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT
				, sizeof(MemoryBlock) + blockSize) 
				)
			, m_ChunkSize( 
				Math::NewMemoryAligmentOffset<size_type>(EARLGREY_DEFAULT_PAGE_ALIGNMENT, m_InternalBlockSize * numberOfBlocks) 
				)
			, m_BlockHead(
				)
		{
			EARLGREY_ASSERT(m_ChunkSize >= m_InternalBlockSize);
			EARLGREY_ASSERT(m_ChunkSize > 0 && m_InternalBlockSize > 0);
			EARLGREY_ASSERT(NumberOfBlocks() >= numberOfBlocks);
		}
		

		inline size_type InternalChunkSize() const
		{
			return m_ChunkSize;
		}

		inline size_type ChunkSize() const
		{
			return BlockSize() * NumberOfBlocks();
		}

		inline size_type InternalBlockSize() const
		{
			return m_InternalBlockSize;
		}

		inline size_type BlockSize() const
		{
			return m_InternalBlockSize - sizeof(MemoryBlock);
		}

		// \note 이 값이 의미가 있나? 어차피 Alloc할 때마다 메모리 블록을 빼가는데?
		inline size_type NumberOfBlocks() const
		{
			return m_ChunkSize / m_InternalBlockSize;
		}

		//! \note Free는 없나?
		inline MemoryBlock * Alloc()
		{
			for(;;)
			{
				MemoryBlock * block = reinterpret_cast<MemoryBlock*>(InterlockedPopEntrySinglyList(&m_BlockHead));
				if(block != NULL)
				{
#ifdef _DEBUG
					SetDebugBit(block, std::numeric_limits<UINT>::max EARLGREY_PREVENT_MACRO_SUBSTITUTION ());
#endif
					return block;
				}

				CreateFreeNodes();
			}
		}

		inline void Free(MemoryBlock * block)
		{
			EARLGREY_ASSERT(block->BlockSize() == BlockSize());

#ifdef _DEBUG
			SetDebugBit(block, std::numeric_limits<UINT>::min EARLGREY_PREVENT_MACRO_SUBSTITUTION ());
#endif

			InterlockedPushEntrySinglyList(&m_BlockHead, &block->m_Item);
		}

		static MemoryBlock * AllocLargeObjectHeap(size_type bytes, DWORD alignment = EARLGREY_DEFAULT_PAGE_ALIGNMENT);
		static void FreeLargeObjectHeap(MemoryBlock * block);

	private:

#ifdef _DEBUG
		static void SetDebugBit(MemoryBlock * block, int bits)
		{
			const size_type bytes = block->BlockSize();
			memset(block->Data(), bits, bytes / sizeof(char));
		}
#endif

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
			EARLGREY_ASSERT(node->m_BlockSize == BlockSize());
			InterlockedPushEntrySinglyList(&m_BlockHead, &node->m_Item);
		}
		

	private:
		size_type m_InternalBlockSize;
		size_type m_ChunkSize;

		SinglyListHead m_BlockHead;

	};

	template<class Allocator>
	class MemoryAllocatorThreadSafeProxy
	{
	public:
		typedef typename Allocator::size_type size_type;

		explicit MemoryAllocatorThreadSafeProxy()
		{
			ScopedLock scopeLock(&m_AllocatorLock);
			m_Allocator = new Allocator;
		}

		~MemoryAllocatorThreadSafeProxy()
		{
			ScopedLock scopeLock(&m_AllocatorLock);
			delete m_Allocator;
			m_Allocator = NULL;
		}

		void * Alloc(size_type bytes)
		{
			ScopedLock scopeLock(&m_AllocatorLock);
			return m_Allocator->Alloc(bytes);
		}

		inline void Free(void * ptr)
		{
			ScopedLock scopeLock(&m_AllocatorLock);
			m_Allocator->Free(ptr);
		}

		Allocator* GetRealAllocator() { return m_Allocator; }

	private:
		Allocator* m_Allocator;
		Loki::Mutex  m_AllocatorLock;
	};
}
