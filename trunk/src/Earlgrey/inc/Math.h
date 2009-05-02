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
			// 내장 함수를 사용하는 프로그램은 함수 호출의 오버헤드가 없기 때문에 빠르게 실행되지만
			// 추가 코드를 만들기 때문에 프로그램이 커질 수 있습니다.
			EARLGREY_ASSERT(n > 0);

			unsigned long index;
			EARLGREY_VERIFY(_BitScanReverse(&index, n));
			return index;
		}

		template<typename DoubleType>
		inline DoubleType Log2Ex(DoubleType n)
		{
			EARLGREY_ASSERT(n > 0);
			const DoubleType TWO = 2;
			return log(n) / log(TWO);
		}


		//! 컴파일 타임에 계산이 가능하고 제일 빠르다. 단 N은 상수만 허용된다.
		template<unsigned N>
		inline unsigned Log2Static()
		{
			return  1 + Log2Static<N / 2>( );
		}

		template<>
		inline unsigned Log2Static<2>()
		{
			return 1UL;
		}

		template<>
		inline unsigned Log2Static<1>()
		{
			return 0UL;
		}

		// x의 N 승
		template< unsigned N >  
		inline double PowStatic( double x )  
		{
			return PowStatic<N % 2>( x )  *  PowStatic<N / 2>( x * x );
		}

		template<>
		inline double PowStatic<1U>( double x ) 
		{ 
			return x;  
		}

		template<>
		inline double PowStatic<0U>( double x )  
		{  
			UNREFERENCED_PARAMETER(x);
			return 1.;  
		}

	}
}
