#include "stdafx.h"
#include "MemoryBlock.h"

namespace Earlgrey
{
	void SuperMemoryBlock::CreateFreeNodes()
	{
		BYTE * newChunk = static_cast<BYTE*>( AllocChunk() );

		const size_type count = NumberOfBlocks();
		EARLGREY_ASSERT(count > 0);

		for (size_type i = 0; i < count; i++)
		{
			MemoryBlock * node = reinterpret_cast<MemoryBlock*>(newChunk + m_InternalBlockSize * i);
			EARLGREY_ASSERT(node != NULL);

			new( reinterpret_cast<void*>(node)) MemoryBlock(m_InternalBlockSize); // ������ ȣ��
			CreateFreeNode(node);
		}
	}
}