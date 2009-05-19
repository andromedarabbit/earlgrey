#pragma once
#include "EarlgreyAssert.h"
#include "EarlgreyMath.h"

namespace Earlgrey
{	
	namespace Math
	{
		template <typename IntType>
		inline IntType NewMemoryOffset(IntType offset, IntType align)
		{
			EARLGREY_ASSERT(offset > 0);
			EARLGREY_ASSERT(offset > align);
			EARLGREY_ASSERT( 
			EARLGREY_STATIC_ASSERT(std::numeric_limits<UIntType>::is_integer == true);
			// EARLGREY_STATIC_ASSERT(std::numeric_limits<UIntType>::is_signed == true);


			return align + ((offset - 1) & ~(align - 1));
		}

	}
}