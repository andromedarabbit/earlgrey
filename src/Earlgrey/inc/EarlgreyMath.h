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
				// ���� �Լ��� ����ϴ� ���α׷��� �Լ� ȣ���� ������尡 ���� ������ ������ ���������
				// �߰� �ڵ带 ����� ������ ���α׷��� Ŀ�� �� �ֽ��ϴ�.
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



		//! ������ Ÿ�ӿ� ����� �����ϰ� ���� ������. �� N�� ����� ���ȴ�.
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

		//! x�� N ��
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

		//! \note �ϴ� ������ �����Ѵ�. 1/2, 1/4 �̷� ���� �˻��� ���� ���� ����.
		template<typename IntType>
		inline BOOL IsPowerOf2(IntType n)
		{
			EARLGREY_ASSERT(std::numeric_limits<IntType>::is_integer == true);
			EARLGREY_ASSERT(n >= 0);

			return n > 0 && (n & (n-1)) == 0;
		}
	}
}
