#include "stdafx.h"
#include "MemoryBlock.h"


#include "EarlgreyMath.h"

namespace Earlgrey
{
	namespace Test
	{
		//! \todo NumberOfBlocks의 개수가 내부 MemoryBlock이 차지하는 공간 때문에 작게 나온다.
		// 차라리 생성자에서 chunkSize 대신 블락의 개수를 받는 편이 낫지 않을까?
		// 물론 지금 구현은 메모리 사용량을 명시한다는 점이 좋긴 하다.
		TEST(MemoryBlockTest, ConstructorWithChunkSize1)
		{
			SuperMemoryBlock::size_type chunkSize = 4096;
			SuperMemoryBlock::size_type blockSize = 64;
			SuperMemoryBlock superBlock(chunkSize, blockSize);

			ASSERT_EQ(static_cast<size_t>(64), superBlock.BlockSize());
			ASSERT_EQ(static_cast<size_t>(4096), superBlock.ChunkSize());
			ASSERT_GT(static_cast<size_t>(64), superBlock.NumberOfBlocks());
		}

		TEST(MemoryBlockTest, ConstructorWithChunkSize2)
		{
			SuperMemoryBlock::size_type chunkSize = 4095;
			SuperMemoryBlock::size_type blockSize = 63;
			SuperMemoryBlock superBlock(chunkSize, blockSize);

			ASSERT_EQ(static_cast<size_t>(64), superBlock.BlockSize());
			ASSERT_EQ(static_cast<size_t>(4096), superBlock.ChunkSize());
			ASSERT_GT(static_cast<size_t>(64), superBlock.NumberOfBlocks());
		}

		
	}

}