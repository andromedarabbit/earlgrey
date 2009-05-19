#pragma once
#include "EarlgreyAssert.h"
#include "EarlgreyMath.h"

namespace Earlgrey
{	
	namespace Math
	{
		//! \todo �޸� ������ ���õ� ���� ����. ���߿� ������ ���Ϸ� �ű��.
		template<typename IntType>
		inline IntType NewOffsetForMemoryAligment(IntType alignment, IntType offset)
		{
			EARLGREY_ASSERT(offset > 0);
			EARLGREY_ASSERT(std::numeric_limits<IntType>::is_integer == true);
			return alignment + ((offset - 1) & ~(alignment - 1));
		}
	}
}