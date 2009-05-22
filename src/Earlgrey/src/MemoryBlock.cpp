#include "stdafx.h"
#include "MemoryBlock.h"

namespace Earlgrey
{
	MemoryBlock * SuperMemoryBlock::AllocLargeObjectHeap(size_type bytes, DWORD alignment)
	{
		MemoryBlock * block = static_cast<MemoryBlock*>(
			_aligned_malloc(sizeof(MemoryBlock) + bytes, alignment)
			);
		EARLGREY_ASSERT(block != NULL);

		new( reinterpret_cast<void*>(block)) MemoryBlock(bytes);

#ifdef _DEBUG
		SetDebugBit(block, std::numeric_limits<UINT>::max());
#endif
		return block;
	}

	void SuperMemoryBlock::FreeLargeObjectHeap(MemoryBlock * memoryBlock)
	{
		EARLGREY_ASSERT(memoryBlock != NULL);

		size_t blockSize = memoryBlock->BlockSize();
		EARLGREY_ASSERT(blockSize > 0);

#ifdef _DEBUG // 이 코드가 필요할까?
		SetDebugBit(memoryBlock, std::numeric_limits<UINT>::min());
#endif

		_aligned_free(memoryBlock);
	}

	void SuperMemoryBlock::CreateFreeNodes()
	{
		BYTE * newChunk = static_cast<BYTE*>( AllocChunk() );

		const size_type count = NumberOfBlocks();
		EARLGREY_ASSERT(count > 0);

		for (size_type i = 0; i < count; i++)
		{
			MemoryBlock * node = reinterpret_cast<MemoryBlock*>(newChunk + m_InternalBlockSize * i);
			EARLGREY_ASSERT(node != NULL);

			new( reinterpret_cast<void*>(node)) MemoryBlock(BlockSize()); // 생성자 호출
			CreateFreeNode(node);
		}
	}
}