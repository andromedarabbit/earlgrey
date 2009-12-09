#include "stdafx.h"
#include "SlabAllocator.h"

namespace Earlgrey
{
	MemoryBlockChunkPool::MemoryBlockChunkPool() 
	{
		for(DWORD i=0; i<POOL_COUNT; ++i)
		{
			m_ChunkPool[i].m_ChunkSize = SlabAllocator::BucketSize(i);
		}
	}

	SlabAllocator::MemoryBlockPool::MemoryBlockPool()
	{
		m_MemoryBlockChunkList = NULL;
		m_MemoryBlockList = NULL;

		m_BlockSize = 0;
		m_PoolCount = 0;
		m_AllocCount = 0;
		m_PhysicalAlloc = 0;
	}

	void SlabAllocator::MemoryBlockPool::SetGlobalPool(MemoryBlockChunkList* ChunkList)
	{
		if(NULL == ChunkList) return;

		EARLGREY_ASSERT(ChunkList->GetChunkSize() == m_BlockSize);
		if(ChunkList->GetChunkSize() == m_BlockSize)
		{
			m_MemoryBlockChunkList = ChunkList;
		}
	}

	BOOL SlabAllocator::MemoryBlockPool::AllocBlocksFromChunkList()
	{
		if(NULL == m_MemoryBlockChunkList) return FALSE;

		MemoryBlockChunk Chunk;
		if(FALSE == m_MemoryBlockChunkList->PopChunk(Chunk)) return FALSE;

		EARLGREY_ASSERT(Chunk.ChunkSize == m_BlockSize);
		if(Chunk.ChunkSize < m_BlockSize) return FALSE;

		//debugf(_T("TLSMemory : Thread[%d] <- Global Queue %u x %u"), ThreadId, m_BlockSize, Chunk.BlockCount()));

		Chunk.Tail->Next = m_MemoryBlockList;
		m_MemoryBlockList = Chunk.Head;

		m_PoolCount += Chunk.BlockCount;

		return TRUE;
	}

	BOOL SlabAllocator::MemoryBlockPool::AllocBlocksFromVirtualAlloc()
	{
		DWORD Blocks  = SLAB_SIZE / m_BlockSize; 
		DWORD Bytes   = Blocks * m_BlockSize;
		DWORD UserBlockSize = m_BlockSize - sizeof(SimpleMemoryBlock);
		SimpleMemoryBlock* RegionPointer = (SimpleMemoryBlock*)VirtualAlloc( NULL, Bytes, MEM_COMMIT, PAGE_READWRITE );
		if( NULL == RegionPointer ) return FALSE;

		m_MemoryBlockList = RegionPointer;
		void* EndOfRegion = ((BYTE*)RegionPointer) + Bytes - 1;
		for(;;)
		{
			RegionPointer->BlockSize = UserBlockSize;
			void* NextPointer = ((BYTE*)RegionPointer) + m_BlockSize;
			if(NextPointer > EndOfRegion)
			{
				RegionPointer->Next = NULL;
				break;
			}
			RegionPointer = RegionPointer->Next = reinterpret_cast<SimpleMemoryBlock*>(NextPointer);
		}

		m_PoolCount += Blocks;
		m_PhysicalAlloc += Math::NewMemoryAligmentPadding(Bytes, (DWORD)GetSystemPageSize());

		return TRUE;
	}

	SimpleMemoryBlock* SlabAllocator::MemoryBlockPool::PopMemoryBlock()
	{
		if(NULL == m_MemoryBlockList)
		{
			BOOL AllocResult = AllocBlocksFromChunkList() || AllocBlocksFromVirtualAlloc();
			if(FALSE == AllocResult)
			{
				throw std::bad_alloc();
			}
		}

		SimpleMemoryBlock* Block = m_MemoryBlockList;
		m_MemoryBlockList = m_MemoryBlockList->Next;

		++m_AllocCount;
		++m_TotalAllocCount;
		--m_PoolCount;
		EARLGREY_ASSERT(m_PoolCount >= -1);

		return Block;
	}

	BOOL SlabAllocator::MemoryBlockPool::HasExtraMargin()
	{
		// TODO(initialjk@googlewave.com): 일단 한번 할당받는 단위의 4배. 최적화 필요.
		return m_PoolCount > (INT)((SLAB_SIZE / m_BlockSize) * 4);
	}

	MemoryBlockChunk SlabAllocator::MemoryBlockPool::TakeOutChunk(DWORD size)
	{
		MemoryBlockChunk Chunk;
		if(NULL == m_MemoryBlockList) return Chunk;

		SimpleMemoryBlock* Block = m_MemoryBlockList;
		Chunk.Head = Block;
		DWORD i;
		for(i=1; (i<size) && (NULL != Block->Next); ++i)
		{
			Block = Block->Next;
		}

		m_MemoryBlockList = Block->Next;
		Block->Next = NULL;
		Chunk.Tail = Block;
		Chunk.ChunkSize = m_BlockSize;
		Chunk.BlockCount = i;
		m_PoolCount -= i;

		//	debugf(_T("TLSMemory : Thread[%d] -> Global Queue %u x %u (Pool = %u)"), THREAD_ID, mBlockSize, i, mPoolCount);

		return Chunk;
	}

	void SlabAllocator::MemoryBlockPool::PushMemoryBlock(SimpleMemoryBlock* InMemoryBlock)
	{
		EARLGREY_ASSERT(NULL != InMemoryBlock);
		if(NULL == InMemoryBlock) return;

		EARLGREY_ASSERT(InMemoryBlock->BlockSize == (m_BlockSize - sizeof(SimpleMemoryBlock))); 

		InMemoryBlock->Next = m_MemoryBlockList;
		m_MemoryBlockList = InMemoryBlock;

		--m_AllocCount;
		++m_PoolCount;

		BOOL SkipEnough = !(m_PoolCount&0xF);
		if(m_MemoryBlockChunkList && SkipEnough && HasExtraMargin())
		{
			MemoryBlockChunk Chunk = TakeOutChunk((SLAB_SIZE / m_BlockSize) * 2); // 이 수치도 최적화 필요
			if(NULL != Chunk.Head)
			{
				m_MemoryBlockChunkList->PushChunk(Chunk);
			}
		}
	}

	SlabAllocator::SlabAllocator()
		:	m_PhysicalCurrentBytes( 0 )
		,	m_UsedCurrentBytes( 0 )
		,	m_UsedPeakBytes( 0 )
		,	m_CurrentAllocs( 0 )
		,	m_TotalAllocs( 0 )
		,	m_MemTime( 0.0 )
		,	m_PageSize( 0 )
	{
		for( DWORD i=0; i<POOL_COUNT; i++ )
		{
			m_Pool[i].m_MemoryBlockList = NULL;
			m_Pool[i].m_BlockSize = BucketSize(i);
		}

		for( DWORD i=0; i<POOL_MAX; i++ )
		{
			DWORD RequiredSize = i + sizeof(SimpleMemoryBlock);
			DWORD Index;
			for(Index=0; m_Pool[Index].BlockSize()<RequiredSize; Index++ ) {};
			EARLGREY_ASSERT(Index<POOL_COUNT);
			m_MemSizeToPool[i] = &m_Pool[Index];
		}
		EARLGREY_ASSERT(POOL_MAX+sizeof(SimpleMemoryBlock)-1 == m_Pool[POOL_COUNT-1].BlockSize());
	}

	void SlabAllocator::SetGlobalPool(MemoryBlockChunkPool* GlobalPool)
	{
		EARLGREY_ASSERT(NULL != GlobalPool);
		if(NULL == GlobalPool) 
		{
			for( DWORD i=0; i<POOL_COUNT; i++ )
			{
				m_Pool[i].SetGlobalPool(GlobalPool->ChunkQueue(i));
			}	
		}
		else
		{
			for( DWORD i=0; i<POOL_COUNT; i++ )
			{
				m_Pool[i].SetGlobalPool(NULL);
			}	
		}
	}

	void* SlabAllocator::Alloc( size_type Size )
	{
		SimpleMemoryBlock* Free;
		if( Size < POOL_MAX )
		{
			Free = m_MemSizeToPool[Size]->PopMemoryBlock();
			EARLGREY_ASSERT(NULL != Free);
		}
		else
		{
			// TODO(initialjk@googlewave.com): Large memory 성능 이슈 생기면 이것도 RB tree써서 대강 pooling. 실은 pool size를 이까지 키우는 게 맞겠지만.
			size_type AlignedSize = Math::NewMemoryAligmentOffset(Size + sizeof(SimpleMemoryBlock), (size_type)GetSystemPageSize()); 
			Free = (SimpleMemoryBlock*)VirtualAlloc( NULL, AlignedSize, MEM_COMMIT, PAGE_READWRITE ); 
			if( NULL == Free )
			{
				throw std::bad_alloc();
			}

			Free->BlockSize = AlignedSize - sizeof(SimpleMemoryBlock);
			Free->Next = NULL;
		}
		return Free->Data();
	}

	void* SlabAllocator::Realloc( void* Ptr, size_type NewSize )
	{
		if( 0 == NewSize )
		{
			Free( Ptr );
			return NULL;
		}

		if( NULL == Ptr )
		{
			return Alloc( NewSize );
		}

		SimpleMemoryBlock* Block = ((SimpleMemoryBlock*)Ptr)-1;
		if(NewSize < Block->BlockSize)
		{
			return Ptr;
		}

		void* NewPtr = Alloc( NewSize );
		memcpy( NewPtr, Ptr, std::min(NewSize, Block->BlockSize) );
		Free( Ptr );

		return NewPtr;
	}

	void SlabAllocator::Free( void* Ptr )
	{
		if( NULL == Ptr )
		{
			return;
		}

		SimpleMemoryBlock* Block = ((SimpleMemoryBlock*)Ptr)-1;
		if( Block->BlockSize < POOL_MAX )
		{
			m_MemSizeToPool[Block->BlockSize]->PushMemoryBlock(Block);
		}
		else
		{
			EARLGREY_VERIFY( VirtualFree( Block, 0, MEM_RELEASE ) );
		}
	}
}