#include "stdafx.h"
#include "MemoryBlock.h"


#include "EarlgreyMath.h"

namespace Earlgrey
{
	namespace Test
	{
		//! \todo NumberOfBlocks�� ������ ���� MemoryBlock�� �����ϴ� ���� ������ �۰� ���´�.
		// ���� �����ڿ��� chunkSize ��� ������ ������ �޴� ���� ���� ������?
		// ���� ���� ������ �޸� ��뷮�� �����Ѵٴ� ���� ���� �ϴ�.
		TEST(MemoryBlockTest, ConstructorWithChunkSize1)
		{
			SuperMemoryBlock::size_type blockSize = 64;
			unsigned short numberOfBlocks = 64;

			SuperMemoryBlock superBlock(blockSize, numberOfBlocks);

			ASSERT_EQ(static_cast<size_t>(64), superBlock.BlockSize());
			ASSERT_LE(static_cast<size_t>(4096), superBlock.ChunkSize());
			ASSERT_LE(static_cast<size_t>(64), superBlock.NumberOfBlocks());
		}

		TEST(MemoryBlockTest, ConstructorWithChunkSize2)
		{
			SuperMemoryBlock::size_type blockSize = 63;
			unsigned short numberOfBlocks = 63;
			SuperMemoryBlock superBlock(blockSize, numberOfBlocks);

			ASSERT_EQ(static_cast<size_t>(64), superBlock.BlockSize());
			ASSERT_LE(static_cast<size_t>(4096), superBlock.ChunkSize());
			ASSERT_LE(static_cast<size_t>(64), superBlock.NumberOfBlocks());
		}

		TEST(MemoryBlockTest, ConstructorWithChunkSize3)
		{
			SuperMemoryBlock::size_type chunkSize = 4096;
			SuperMemoryBlock::size_type blockSize = 64;

			SuperMemoryBlock superBlock(chunkSize, blockSize);

			ASSERT_EQ(static_cast<size_t>(64), superBlock.BlockSize());
			ASSERT_GT(static_cast<size_t>(4096), superBlock.ChunkSize());
			ASSERT_EQ(static_cast<size_t>(4096), superBlock.InternalChunkSize());
			ASSERT_GT(static_cast<size_t>(64), superBlock.NumberOfBlocks());
		}

		TEST(MemoryBlockTest, ConstructorWithChunkSize4)
		{
			SuperMemoryBlock::size_type chunkSize = 4095;
			SuperMemoryBlock::size_type blockSize = 63;
			SuperMemoryBlock superBlock(chunkSize, blockSize);

			ASSERT_EQ(static_cast<size_t>(64), superBlock.BlockSize());
			ASSERT_GT(static_cast<size_t>(4096), superBlock.ChunkSize());
			ASSERT_EQ(static_cast<size_t>(4096), superBlock.InternalChunkSize());
			ASSERT_GT(static_cast<size_t>(64), superBlock.NumberOfBlocks());
		}
	}

}