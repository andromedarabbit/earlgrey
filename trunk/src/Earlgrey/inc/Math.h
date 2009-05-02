#pragma once
#include "EarlgreyAssert.h"
#include <math.h> // log()
#include <intrin.h> // _BitScanReverse

#pragma intrinsic(_BitScanReverse)

namespace Earlgrey
{
	namespace Math
	{
		template<typename IntType>
		inline IntType Log2(IntType n) // For double, long double, float
		{
			EARLGREY_ASSERT(n > 0);

			unsigned long index;
			EARLGREY_VERIFY(_BitScanReverse(&index, n));
			return index;
		}

		/*
		inline unsigned long Log2(unsigned long n)
		EARLGREY_ASSERT(n > 0);

		unsigned long index;
		EARLGREY_VERIFY(_BitScanReverse(&index, n));
		return index;*/

		template<typename DoubleType>
		inline DoubleType Log2Ex(DoubleType n)
		{
			EARLGREY_ASSERT(n > 0);
			const DoubleType TWO = 2;
			return log(n) / log(TWO);
		}
	}
}
