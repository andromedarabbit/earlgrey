#include "stdafx.h"
#include "MemoryBlock.h"


#include "EarlgreyMath.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(MemoryBlockTest, Case1)
		{
			SuperMemoryBlock::size_type chunkSize = 4096;
			SuperMemoryBlock::size_type blockSize = 64;
			SuperMemoryBlock superBlock(chunkSize, blockSize);

			ASSERT_GE(64, superBlock.BlockSize());
		}
	}
}