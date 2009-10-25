#pragma once
#include "EarlgreyAssert.h"
#include "EarlgreyMath.h"

namespace Earlgrey
{	
	namespace Math
	{
		//! \todo 메모리 구현과 관련된 수학 연산. 나중에 별도의 파일로 옮긴다.
		template<typename IntType>
		inline IntType NewMemoryAligmentOffset(IntType alignment, IntType offset)
		{
			EARLGREY_ASSERT(offset >= 0);
			EARLGREY_ASSERT(std::numeric_limits<IntType>::is_integer == true);

			IntType newOffset = alignment + ((offset - 1) & ~(alignment - 1));
			EARLGREY_ASSERT(newOffset >= offset);
			return newOffset;
		}
		
		template<typename IntType>
		inline IntType NewMemoryAligmentPadding(IntType alignment, IntType offset)
		{
			IntType newOffset = NewMemoryAligmentOffset(alignment, offset);
			return newOffset - offset;
		}
		


	}
}