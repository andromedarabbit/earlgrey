#pragma once
#include "EarlgreyAssert.h"
#include <math.h> // log()
#include <intrin.h> // _BitScanReverse

#pragma intrinsic(_BitScanReverse)

namespace Earlgrey
{
	namespace Math
	{
		//! \note boost의 numeric_cast와 교체하기 쉽게 이름을 똑같이 지었다.
		//! \todo boost처럼 NaN, Infinite 등을 모두 고려하면 좋겠다.
		template<typename Target, typename Source>
		inline Target numeric_cast(Source no) throw(...)
		{
			EARLGREY_STATIC_ASSERT(
				std::numeric_limits<Target>::is_integer == true
				&& std::numeric_limits<Source>::is_integer == true
				);

/*
			template<bool condition,class Then,class Else>
			struct IF 
			{ 
				// typedef Then RET; 


			};

			template<class Then,class Else>
			// Specialized template for condition=false
			struct IF<false,Then,Else> 
			{ 
				typedef Else RET; 
			};


			std::numeric_limits<Target>::is_signed() != std::numeric_limits<Source>::is_signed()
				&& sizeof(Target) == sizeof(Source)
*/
			if(no > std::numeric_limits<Target>::max())
			{
				throw std::overflow_error("bad numeric conversion: overflow");
			}

			if(no < std::numeric_limits<Target>::min())
			{
				throw std::underflow_error("bad numeric conversion: underflow");
			}

			return static_cast<Target>( no );
		}
		

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

	}
}
