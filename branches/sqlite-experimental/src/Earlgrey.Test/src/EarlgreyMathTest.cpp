#include "stdafx.h"
#include "EarlgreyMath.h"
#include "Macros.h"

#include <type_traits> 

namespace Earlgrey
{
	namespace Test
	{	
		/*
		TEST(EarlgreyMathTest, SameTypes) // 이 테스트 케이스는 컴파일 실패가 나야 정상이다.
		{
			EARLGREY_NUMERIC_CAST<int>(INT_MAX);
		}
		*/
		TEST(TR1, IsSame)
		{
			bool isSame = std::tr1::is_same<int, int>::value;
			ASSERT_TRUE(isSame);
			isSame = std::tr1::is_same<long, long>::value;
			ASSERT_TRUE(isSame);
			isSame = std::tr1::is_same<float, float>::value;
			ASSERT_TRUE(isSame);

			isSame = std::tr1::is_same<int, float>::value;
			ASSERT_FALSE(isSame);
			isSame = std::tr1::is_same<double, float>::value;
			ASSERT_FALSE(isSame);

			struct IntWrapper
			{
				int intMember;
			};

			isSame = std::tr1::is_same<IntWrapper, int>::value;
			ASSERT_FALSE(isSame);

			struct AnotherIntWrapper : IntWrapper
			{
			};

			isSame = std::tr1::is_same<IntWrapper, AnotherIntWrapper>::value;
			ASSERT_FALSE(isSame);

			struct JustAnotherIntWrapper
			{
				int intMember;
			};

			isSame = std::tr1::is_same<JustAnotherIntWrapper, AnotherIntWrapper>::value;
			ASSERT_FALSE(isSame);
		}

		TEST(EarlgreyMathTest, Log2ByUsingIntrinsicFunction)
		{
			unsigned long n1 = 64;
			ASSERT_EQ(6, Earlgrey::Math::Log2(n1));

			unsigned long n2 = 1;
			ASSERT_EQ(0, Earlgrey::Math::Log2(n2));

			unsigned long n3 = 1025;
			ASSERT_EQ(10, Earlgrey::Math::Log2(n3));

			unsigned long n4 = std::numeric_limits<unsigned long>::max EARLGREY_PREVENT_MACRO_SUBSTITUTION ();
			ASSERT_EQ(31, Earlgrey::Math::Log2(n4));

			unsigned long n5 = 3;
			ASSERT_EQ(1, Earlgrey::Math::Log2(n5));
		}

		TEST(EarlgreyMathTest, Log2)
		{
			double n1 = 64;
			INT64 retValue1 = static_cast<INT64>(Earlgrey::Math::Log2(n1));
			ASSERT_EQ(6, retValue1);

			float n2 = 1;
			INT64 retValue2 = static_cast<INT64>(Earlgrey::Math::Log2(n2));
			ASSERT_EQ(0, retValue2);

			long double n3 = 1025;
			INT64 retValue3 = static_cast<INT64>(Earlgrey::Math::Log2(n3));
			ASSERT_EQ(10, retValue3);

			long double n4 = std::numeric_limits<unsigned long>::max EARLGREY_PREVENT_MACRO_SUBSTITUTION ();
			INT64 retValue4 = static_cast<INT64>(Earlgrey::Math::Log2(n4));
			ASSERT_EQ(31, retValue4);

			long double n5 = 3;
			INT64 retValue5 = static_cast<INT64>(Earlgrey::Math::Log2(n5));
			ASSERT_EQ(1, retValue5);
		}

		TEST(EarlgreyMathTest, Log3)
		{
			const unsigned long n1 = 64;
			ASSERT_EQ(6, Earlgrey::Math::Log2Static<n1>());

			const unsigned long n2 = 1;
			ASSERT_EQ(0, Earlgrey::Math::Log2Static<n2>());

			const unsigned long n3 = 1025;
			ASSERT_EQ(10, Earlgrey::Math::Log2Static<n3>());

			const unsigned long n4 = ULONG_MAX;
			ASSERT_EQ(n4, (std::numeric_limits<unsigned long>::max)());
			ASSERT_EQ(31, Earlgrey::Math::Log2Static<n4>());			

			const unsigned long n5 = 3;
			ASSERT_EQ(1, Earlgrey::Math::Log2Static<n5>());
		}

		TEST(EarlgreyMathTest, PowStatic1)
		{
			ASSERT_EQ(1, Earlgrey::Math::PowStatic<0>(0));

			ASSERT_EQ(1, Earlgrey::Math::PowStatic<0>(2));
			ASSERT_EQ(2, Earlgrey::Math::PowStatic<1>(2));
			ASSERT_EQ(4, Earlgrey::Math::PowStatic<2>(2));
			ASSERT_EQ(8, Earlgrey::Math::PowStatic<3>(2));
			ASSERT_EQ(16, Earlgrey::Math::PowStatic<4>(2));
			
			ASSERT_EQ(32768, Earlgrey::Math::PowStatic<15>(2));
		}

		TEST(EarlgreyMathTest, PowStatic2)
		{
			size_t ret = (size_t)Earlgrey::Math::PowStatic<15>(2);
			ASSERT_EQ(32768, ret);
		}

		TEST(EarlgreyMathTest, IsPowerOf2)
		{
			ASSERT_FALSE(Earlgrey::Math::IsPowerOf2(0));
			ASSERT_TRUE(Earlgrey::Math::IsPowerOf2(1));
			ASSERT_TRUE(Earlgrey::Math::IsPowerOf2(2));
			ASSERT_TRUE(Earlgrey::Math::IsPowerOf2(64));
			ASSERT_TRUE(Earlgrey::Math::IsPowerOf2(128));
			ASSERT_TRUE(Earlgrey::Math::IsPowerOf2(1024));

			ASSERT_FALSE(Earlgrey::Math::IsPowerOf2(3));
			ASSERT_FALSE(Earlgrey::Math::IsPowerOf2(255));
			ASSERT_FALSE(Earlgrey::Math::IsPowerOf2(2049));
		}
	}
}