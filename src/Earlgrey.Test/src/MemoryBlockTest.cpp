#include "stdafx.h"
#include "MemoryBlock.h"


#include "EarlgreyMath.h"

namespace Earlgrey
{
	namespace Test
	{
		//! \todo NumberOfBlocks�� ������ ���� MemoryBlock�� �����ϴ� ���� ������ �۰� ���´�.
		// ���� �����ڿ��� chunkSize ��� ����� ������ �޴� ���� ���� ������?
		// ���� ���� ������ �޸� ��뷮�� ����Ѵٴ� ���� ���� �ϴ�.
		TEST(MemoryBlockTest, Case1)
		{
			SuperMemoryBlock::size_type chunkSize = 4096;
			SuperMemoryBlock::size_type blockSize = 64;
			SuperMemoryBlock superBlock(chunkSize, blockSize);

			ASSERT_EQ(64, superBlock.BlockSize());
			ASSERT_GE(64, superBlock.NumberOfBlocks());
		}

		TEST(MemoryBlockTest, Case2)
		{
			SuperMemoryBlock::size_type chunkSize = 4095;
			SuperMemoryBlock::size_type blockSize = 63;
			SuperMemoryBlock superBlock(chunkSize, blockSize);

			ASSERT_EQ(64, superBlock.BlockSize());
			ASSERT_EQ(4096, superBlock.ChunkSize());
			ASSERT_GE(64, superBlock.NumberOfBlocks());
		}

		
	}

}