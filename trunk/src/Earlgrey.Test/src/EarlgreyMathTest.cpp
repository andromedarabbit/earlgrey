#include "stdafx.h"
#include "EarlgreyMath.h"
#include "Macros.h"

namespace Earlgrey
{
	namespace Test
	{	
		TEST(EarlgreyMathTest, NumericCast) 
		{
			struct Mock
			{
				static void Overflow()
				{
					INT16 i = Earlgrey::Math::numeric_cast<INT16>(INT_MAX);
					DBG_UNREFERENCED_LOCAL_VARIABLE(i);
				}

				static void Underflow()
				{
					INT16 i = Earlgrey::Math::numeric_cast<INT16>(INT_MIN);
					DBG_UNREFERENCED_LOCAL_VARIABLE(i);
				}
			};
			
			ASSERT_THROW(Mock::Overflow(),  std::overflow_error);
			ASSERT_THROW(Mock::Underflow(),  std::underflow_error);

		}


		TEST(EarlgreyMathTest, NumericCastInTheCaseOfSignMismatch1) 
		{
			short source = 1;
			unsigned short target = Earlgrey::Math::numeric_cast<unsigned short>(source);
			ASSERT_EQ( static_cast<unsigned short>(1), target);
		}


		TEST(EarlgreyMathTest, NumericCastInTheCaseOfSignMismatch2) 
		{
			unsigned short source = 1;
			short target = Earlgrey::Math::numeric_cast<short>(source);
			ASSERT_EQ( static_cast<short>(1), target);
		}

		TEST(EarlgreyMathTest, NumericCastInTheCaseOfSignMismatchAndSameSize1) 
		{
			DWORD source = 1;
			int target = Earlgrey::Math::numeric_cast<int>(source);
			ASSERT_EQ( static_cast<int>(1), target);
		}

		TEST(EarlgreyMathTest, NumericCastInTheCaseOfSignMismatchAndSameSize2) 
		{
			int source = 1;
			DWORD target = Earlgrey::Math::numeric_cast<DWORD>(source);
			ASSERT_EQ( static_cast<DWORD>(1), target);
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

		TEST(EarlgreyMathTest, Log2Ex)
		{
			double n1 = 64;
			INT64 retValue1 = static_cast<INT64>(Earlgrey::Math::Log2Ex(n1));
			ASSERT_EQ(6, retValue1);

			float n2 = 1;
			INT64 retValue2 = static_cast<INT64>(Earlgrey::Math::Log2Ex(n2));
			ASSERT_EQ(0, retValue2);

			long double n3 = 1025;
			INT64 retValue3 = static_cast<INT64>(Earlgrey::Math::Log2Ex(n3));
			ASSERT_EQ(10, retValue3);

			long double n4 = std::numeric_limits<unsigned long>::max EARLGREY_PREVENT_MACRO_SUBSTITUTION ();
			INT64 retValue4 = static_cast<INT64>(Earlgrey::Math::Log2Ex(n4));
			ASSERT_EQ(31, retValue4);

			long double n5 = 3;
			INT64 retValue5 = static_cast<INT64>(Earlgrey::Math::Log2Ex(n5));
			ASSERT_EQ(1, retValue5);
		}

		TEST(EarlgreyMathTest, Log2Ex2)
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
	}
}