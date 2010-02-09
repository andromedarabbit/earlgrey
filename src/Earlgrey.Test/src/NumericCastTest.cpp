#include "stdafx.h"
#include "numeric_cast.hpp"

#ifdef EARLGREY_BUILD_USE_BOOST

namespace Earlgrey
{
	namespace Test
	{
		TEST(EarlgreyMathTest, OverflowAndUnderflow) 
		{
			struct Mock
			{
				static void Overflow()
				{
					INT16 i = EARLGREY_NUMERIC_CAST<INT16>(INT_MAX);
					DBG_UNREFERENCED_LOCAL_VARIABLE(i);
				}

				static void Underflow()
				{
					INT16 i = EARLGREY_NUMERIC_CAST<INT16>(INT_MIN);
					DBG_UNREFERENCED_LOCAL_VARIABLE(i);
				}
			};

			// ASSERT_THROW(Mock::Overflow(),  std::overflow_error);
			ASSERT_THROW(Mock::Overflow(),  boost::numeric::positive_overflow);
			// ASSERT_THROW(Mock::Underflow(),  std::underflow_error);
			ASSERT_THROW(Mock::Underflow(),  boost::numeric::negative_overflow);
		}

		TEST(EarlgreyMathTest, NumericCastInTheCaseOfSignMismatch1) 
		{
			short source = 1;
			unsigned short target = EARLGREY_NUMERIC_CAST<unsigned short>(source);
			ASSERT_EQ( static_cast<unsigned short>(1), target);
		}

		TEST(EarlgreyMathTest, NumericCastInTheCaseOfSignMismatch2) 
		{
			unsigned short source = 1;
			short target = EARLGREY_NUMERIC_CAST<short>(source);
			ASSERT_EQ( static_cast<short>(1), target);
		}

		TEST(EarlgreyMathTest, NumericCastInTheCaseOfSignMismatchAndSameSize1) 
		{
			DWORD source = 1;
			int target = EARLGREY_NUMERIC_CAST<int>(source);
			ASSERT_EQ( static_cast<int>(1), target);
		}

		TEST(EarlgreyMathTest, NumericCastInTheCaseOfSignMismatchAndSameSize2) 
		{
			int source = 1;
			DWORD target = EARLGREY_NUMERIC_CAST<DWORD>(source);
			ASSERT_EQ( static_cast<DWORD>(1), target);
		}

		TEST(EarlgreyMathTest, NumericCastInTheCaseOfFullySameTypeOnWin32)
		{
			size_t source = 1;
			DWORD target = EARLGREY_NUMERIC_CAST<DWORD>(source);
			ASSERT_EQ( static_cast<DWORD>(1), target );
		}

		TEST(EarlgreyMathTest, NumericCastInTheCaseOfSignMismatchAndSameSize3) 
		{
			struct Mock
			{
				static void Overflow()
				{
					int i = EARLGREY_NUMERIC_CAST<int>(UINT_MAX);
					DBG_UNREFERENCED_LOCAL_VARIABLE(i);
				}

				static void Underflow()
				{
					UINT i = EARLGREY_NUMERIC_CAST<UINT>(INT_MIN);
					DBG_UNREFERENCED_LOCAL_VARIABLE(i);
				}
			};

			// ASSERT_THROW(Mock::Overflow(),  std::overflow_error);
			ASSERT_THROW(Mock::Overflow(),  boost::numeric::positive_overflow);
			// ASSERT_THROW(Mock::Underflow(),  std::underflow_error);
			ASSERT_THROW(Mock::Underflow(),  boost::numeric::negative_overflow);
		}
	}
}

#endif // !EARLGREY_BUILD_USE_BOOST