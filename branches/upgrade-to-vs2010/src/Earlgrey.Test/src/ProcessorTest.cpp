#include "stdafx.h"
#include "Processor.h"

namespace Earlgrey
{
	namespace Test
	{	

		TEST(ProcessorTest, ProcessorCount)
		{
			DWORD count = Processor::ProcessorCount();
			DWORD expected = 1;
			ASSERT_GE(count, expected);
		}

		TEST(ProcessorTest, ProcessorCacheLineSize)
		{		
			const WORD cacheLineSize = Processor::ProcessorCacheLineSize();
			ASSERT_EQ(64, cacheLineSize);
		}

		TEST(ProcessorTest, ActiveProcessorCoresCount)
		{		
			const DWORD count = Processor::ActiveProcessorCoresCount();
			ASSERT_GT(count, static_cast<DWORD>(1));
		}

		TEST(ProcessorTest, IsLittleEndian)
		{		
			// currently only supports 80x86 CPUs
			ASSERT_TRUE2(Processor::IsLittleEndian());
		}

	}
}
