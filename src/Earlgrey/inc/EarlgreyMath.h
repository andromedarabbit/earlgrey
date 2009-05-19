#pragma once
#include "EarlgreyAssert.h"
#include "numeric_cast.hpp"

#include <math.h> // log()
#include <intrin.h> // _BitScanReverse

#pragma intrinsic(_BitScanReverse)

namespace Earlgrey
{
	namespace Math
	{		
		template<typename IntType>
		class Log2WithInteger
		{
			template<typename IntType> friend IntType Log2(IntType no);

			static IntType Calculate(IntType n) // For double, long double, float
			{
				// 내장 함수를 사용하는 프로그램은 함수 호출의 오버헤드가 없기 때문에 빠르게 실행되지만
				// 추가 코드를 만들기 때문에 프로그램이 커질 수 있습니다.
				EARLGREY_ASSERT(n > 0);
				EARLGREY_STATIC_ASSERT(std::numeric_limits<IntType>::is_integer == true);

				unsigned long index;
				EARLGREY_VERIFY(_BitScanReverse(&index, n));
				return index;
			}

		public:
			typedef Log2WithInteger type;
		};

		template<typename FloatType>
		class Log2WithFloat
		{
			template<typename FloatType> friend FloatType Log2(FloatType no);
	
			static FloatType Calculate(FloatType n)
			{
				EARLGREY_ASSERT(n > 0);
				EARLGREY_STATIC_ASSERT(std::numeric_limits<FloatType>::is_integer == false);

				const FloatType TWO = 2;
				return log(n) / log(TWO);
			}

		public:
			typedef Log2WithFloat type;
		};

		template<typename NumericType>
		inline NumericType Log2(NumericType no) 
		{
			typedef
				typename mpl::if_<
				std::numeric_limits<NumericType>::is_integer
				, typename Log2WithInteger<NumericType>::type
				, typename Log2WithFloat<NumericType>::type
				>::type 
				type;
			return type::Calculate(no);
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

		//! x의 N 승
		//! \ref http://cd-docdb.fnal.gov/cgi-bin/RetrieveFile?docid=360&extension=ppt
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

		//! \note 일단 정수만 지원한다. 1/2, 1/4 이런 값을 검사할 일이 당장 없다.
		template<typename IntType>
		inline BOOL IsPowerOf2(IntType n)
		{
			EARLGREY_ASSERT(std::numeric_limits<IntType>::is_integer == true);
			EARLGREY_ASSERT(n >= 0);

			return n > 0 && (n & (n-1)) == 0;
		}
	}
}
