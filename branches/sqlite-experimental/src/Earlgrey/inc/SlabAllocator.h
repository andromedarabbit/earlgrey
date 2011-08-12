#pragma once

#include <memory>
#include "MemoryMath.h"
#include "MemoryBlock.h"
#include "LockfreeQueue.hpp"
#include "SlabAllocator.h"

namespace Earlgrey
{
#define BUCKET_COUNT_OF_32KB 42
	using namespace Algorithm::Lockfree;

	struct MemoryBlockChunk
	{
		SimpleMemoryBlock* Head;
		SimpleMemoryBlock* Tail;
		DWORD ChunkSize;
		DWORD BlockCount;

		explicit MemoryBlockChunk() : Head(NULL), Tail(NULL), ChunkSize(0), BlockCount(0) {}
	};

	class MemoryBlockChunkList
	{
	public:
		MemoryBlockChunkList() : m_ChunkList(NULL) {}
		inline void PushChunk(const MemoryBlockChunk& Chunk);
		inline BOOL PopChunk(MemoryBlockChunk& Chunk);
		inline DWORD GetChunkSize() { return m_ChunkSize; }

	private:
		friend class MemoryBlockChunkPool;
		Queue<MemoryBlockChunk>* m_ChunkList;
		DWORD m_ChunkSize;
	};

	class MemoryBlockChunkPool
	{
	public:
		MemoryBlockChunkPool();
		inline MemoryBlockChunkList* ChunkQueue(DWORD index);

	private:
		enum {POOL_COUNT = BUCKET_COUNT_OF_32KB };

		MemoryBlockChunkList m_ChunkPool[POOL_COUNT];
	};

	class SlabAllocator : private Uncopyable
	{
	public:
		enum {POOL_COUNT = BUCKET_COUNT_OF_32KB };
		enum {POOL_MAX   = 0x8000-sizeof(SimpleMemoryBlock)+1};
		enum {SLAB_SIZE  = 64 * 1024};

		typedef SimpleMemoryBlock::size_type size_type;

		SlabAllocator();

		static inline DWORD BucketSize(DWORD Index);

		void SetGlobalPool(MemoryBlockChunkPool* GlobalPool);

		void* Alloc(size_type bytes);
		void* Realloc(void* ptr, size_type bytes);
		void Free(void * ptr);

	private:
		class MemoryBlockPool
		{
			friend class SlabAllocator;
		public:
			MemoryBlockPool();

			void SetGlobalPool(MemoryBlockChunkList* ChunkList);

			SimpleMemoryBlock* PopMemoryBlock();
			void PushMemoryBlock(SimpleMemoryBlock* InMemoryBlock);

			DWORD BlockSize() { return m_BlockSize; }
			INT PoolCount() { return m_PoolCount; }
			INT AllocCount() { return m_AllocCount; }
			DWORD TotalAllocCount() { return m_TotalAllocCount; }
			DWORD PhysicalAlloc() { return m_PhysicalAlloc; }



		private:
			BOOL AllocBlocksFromChunkList();
			BOOL AllocBlocksFromVirtualAlloc();
			MemoryBlockChunk TakeOutChunk(DWORD size);
			BOOL HasExtraMargin();

			SimpleMemoryBlock* m_MemoryBlockList;
			MemoryBlockChunkList* m_MemoryBlockChunkList;

			DWORD m_BlockSize;
			INT m_PoolCount;
			INT m_AllocCount;
			DWORD m_TotalAllocCount;
			DWORD m_PhysicalAlloc;
		};

		MemoryBlockPool  m_Pool[POOL_COUNT];
		MemoryBlockPool* m_MemSizeToPool[POOL_MAX];

		DWORD   m_PageSize;

		INT m_PhysicalCurrentBytes;
		INT m_UsedCurrentBytes;
		INT m_UsedPeakBytes;
		INT m_CurrentAllocs;
		DWORD m_TotalAllocs;
		FLOAT m_MemTime;
	};

	inline void MemoryBlockChunkList::PushChunk(const MemoryBlockChunk& Chunk)
	{
		EARLGREY_ASSERT(m_ChunkSize == Chunk.ChunkSize);
		if(m_ChunkSize <= Chunk.ChunkSize)
		{
			if(NULL == m_ChunkList)
			{
				Queue<MemoryBlockChunk>* NewChunkList = new Queue<MemoryBlockChunk>();
				InterlockedCompareExchangePointer((volatile PVOID*)&m_ChunkList, (PVOID)NewChunkList, NULL);
				if( NewChunkList != m_ChunkList)
				{
					delete NewChunkList;	
				}
			}

			m_ChunkList->Enqueue(Chunk);
		}
	}

	inline BOOL MemoryBlockChunkList::PopChunk(MemoryBlockChunk& Chunk)
	{
		if(NULL == m_ChunkList) return FALSE;
		BOOL Success = m_ChunkList->Dequeue(Chunk);
		if(FALSE == Success) return FALSE;

		EARLGREY_ASSERT(m_ChunkSize == Chunk.ChunkSize);
		return TRUE;
	}

	inline MemoryBlockChunkList* MemoryBlockChunkPool::ChunkQueue(DWORD index)
	{
		EARLGREY_ASSERT(index < POOL_COUNT);
		return (index < POOL_COUNT)? &m_ChunkPool[index]: NULL;
	}

	inline DWORD SlabAllocator::BucketSize(DWORD Index)
	{
		if(0 == Index) return 8;
		if(Index < 5) return (8<<((Index+1)>>2)) + (2<<Index);

		return (4+((Index+7)&3)) << (1+((Index+7)>>2));
	}
}