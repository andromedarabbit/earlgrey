#include "stdafx.h"
#include "ADOCast.h"

#pragma warning(push)
#pragma warning(disable: 4127)
#include "ATLComTime.h"
#pragma warning(pop)

#undef min
#undef max

namespace Earlgrey {
	namespace ADO {
		namespace Test {

			template <typename T>
			void TestValue(const T srcValue)
			{
				_variant_t var(srcValue);

				const T dstValue = database_cast<T>(var);
				ASSERT_EQ(dstValue, srcValue);

				_variant_t var2(dstValue);
				ASSERT_EQ(srcValue, (T)var2); // _variant_t 가 원래 지원하는 캐스팅

				_variant_t var3 = database_cast<_variant_t>(dstValue);
				ASSERT_EQ(srcValue, (T)var2);
			}

			template <>
			void TestValue<CY>(const CY srcValue)
			{
				_variant_t var(srcValue);

				const CY dstValue = database_cast<CY>(var);
				ASSERT_EQ(dstValue.int64, srcValue.int64);

				_variant_t var2(dstValue);
				CY cy = (CY)var2;
				ASSERT_EQ(srcValue.int64, cy.int64);
			}

			template <>
			void TestValue<DECIMAL>(const DECIMAL srcValue)
			{
				_variant_t var(srcValue);

				const DECIMAL dstValue = database_cast<DECIMAL>(var);
				ASSERT_EQ(dstValue.Hi32, srcValue.Hi32);
				ASSERT_EQ(dstValue.Lo64, srcValue.Lo64);

				_variant_t var2(dstValue);
				DECIMAL decimalValue = (DECIMAL)var2;
				ASSERT_EQ(srcValue.Hi32, decimalValue.Hi32);
				ASSERT_EQ(srcValue.Lo64, decimalValue.Lo64);
			}


			template <typename T>
			void TestNumber()
			{
				const T minValue = std::numeric_limits<T>::min();
				TestValue<T>(minValue);

				const T maxValue = std::numeric_limits<T>::max();
				TestValue<T>(maxValue);

				const T mediumValue = (minValue + maxValue) / 2;
				TestValue<T>(mediumValue);
			}

			template <>
			void TestNumber<CY>()
			{
				CY minValue;
				minValue.int64 = std::numeric_limits<LONGLONG>::min();
				TestValue<CY>(minValue);

				CY maxValue;
				maxValue.int64 = std::numeric_limits<LONGLONG>::max();
				TestValue<CY>(maxValue);

				CY mediumValue;
				mediumValue.int64 = (minValue.int64 + maxValue.int64) / 2;
				TestValue<CY>(mediumValue);
			}

			template <>
			void TestNumber<DECIMAL>()
			{
				DECIMAL minValue;
				minValue.Hi32 = std::numeric_limits<ULONG>::min();
				minValue.Lo64 = std::numeric_limits<ULONGLONG>::min();
				TestValue<DECIMAL>(minValue);

				DECIMAL maxValue;
				maxValue.Hi32 = std::numeric_limits<ULONG>::max();
				maxValue.Lo64 = std::numeric_limits<ULONGLONG>::max();
				TestValue<DECIMAL>(maxValue);

				DECIMAL mediumValue;
				mediumValue.Hi32 = (minValue.Hi32 + maxValue.Hi32) / 2;
				mediumValue.Lo64 = (minValue.Lo64 + maxValue.Lo64) / 2;
				TestValue<DECIMAL>(mediumValue);
			}


			TEST(ADOCastTest, CastShort)
			{
				TestNumber<SHORT>();
			}

			TEST(ADOCastTest, CastLong)
			{
				TestNumber<long>();
			}

			TEST(ADOCastTest, CastFloat)
			{
				TestNumber<float>();
			}

			TEST(ADOCastTest, CastDouble)
			{
				TestNumber<double>();
			}

			TEST(ADOCastTest, CastCy)
			{
				TestNumber<CY>();
			}

			TEST(ADOCastTest, CastEmptyBstr)
			{
				const _bstr_t srcValue(_T(""));
				TestValue<_bstr_t>(srcValue);
			}

			TEST(ADOCastTest, CastBstr)
			{
				const _bstr_t srcValue(_T("가나다라"));
				TestValue<_bstr_t>(srcValue);
			}

			TEST(ADOCastTest, CastWCharString)
			{
				const xwstring srcValue(L"가나다라");
				_variant_t var(srcValue.c_str());

				const xwstring dstValue( database_cast<LPCWSTR>(var) );
				ASSERT_TRUE(dstValue == srcValue);
			}

			TEST(ADOCastTest, CastEmptyWCharString)
			{
				const xwstring srcValue(L"");
				_variant_t var(srcValue.c_str());

				const xwstring dstValue( database_cast<LPCWSTR>(var) );
				ASSERT_TRUE(dstValue == srcValue);
			}

			TEST(ADOCastTest, CastFalse)
			{	
				_variant_t var(false);

				const bool dstValue = database_cast<bool>(var);
				ASSERT_FALSE(dstValue);

				_variant_t var2 = database_cast<_variant_t>(false);
				ASSERT_TRUE(var == var2);

				_variant_t var3 = database_cast<_variant_t>(true);
				ASSERT_TRUE(var != var3);
			}

			TEST(ADOCastTest, CastTrue)
			{	
				_variant_t var(true);

				const bool dstValue = database_cast<bool>(var);
				ASSERT_TRUE(dstValue);

				_variant_t var2 = database_cast<_variant_t>(true);
				ASSERT_TRUE(var == var2);

				_variant_t var3 = database_cast<_variant_t>(false);
				ASSERT_TRUE(var != var3);
			}

			TEST(ADOCastTest, CastCharString)
			{
				const char * srcValue = "가나다라";
				_variant_t var(srcValue);

				const xwstring dstValue( database_cast<const WCHAR *>(var) );
				ASSERT_TRUE(dstValue == _T("가나다라"));

				_variant_t var2 = database_cast<_variant_t>(srcValue);
				ASSERT_TRUE(var == var2);
			}

			TEST(ADOCastTest, CastCharString2)
			{
				const char * srcValue = "가나다라";
				_variant_t var(srcValue);

				const xstring dstValue( database_cast<const CHAR *>(var) );
				ASSERT_TRUE(dstValue == "가나다라");

				_variant_t var2 = database_cast<_variant_t>(srcValue);
				ASSERT_TRUE(var == var2);
			}

			TEST(ADOCastTest, CastDecimal)
			{
				TestNumber<DECIMAL>();
			}

			TEST(ADOCastTest, CastByte)
			{
				TestNumber<BYTE>();
			}

			TEST(ADOCastTest, CastChar)
			{
				const char srcValue = 'A';
				_variant_t var(srcValue);

				const CHAR dstValue( database_cast<CHAR>(var) );
				ASSERT_EQ(dstValue, srcValue);

				_variant_t var2 = database_cast<_variant_t>(srcValue);
				ASSERT_TRUE(var == var2);
			}

			TEST(ADOCastTest, CastUShort)
			{
				TestNumber<USHORT>();
			}

			TEST(ADOCastTest, CastULong)
			{
				TestNumber<ULONG>();
			}

			TEST(ADOCastTest, CastInt)
			{
				TestNumber<int>();
			}

			TEST(ADOCastTest, CastUInt)
			{
				TestNumber<unsigned int>();
			}

			TEST(ADOCastTest, CastLongLong)
			{
				TestNumber<LONGLONG>();
			}

			TEST(ADOCastTest, CastULongLong)
			{
				TestNumber<ULONGLONG>();
			}

			TEST(ADOCastTest, CastDateTime)
			{
				_variant_t src(COleDateTime(2009, 4, 2, 12, 10, 33), VT_DATE);
				DateTime dst = database_cast<DateTime>(src);

				ASSERT_EQ(2009, dst.Year());
				ASSERT_EQ(4, dst.Month());
				ASSERT_EQ(2, dst.Day());
				ASSERT_EQ(12, dst.Hour());
				ASSERT_EQ(10, dst.Minute());
				ASSERT_EQ(33, dst.Second());

				_variant_t var = database_cast<_variant_t>(dst);
				ASSERT_TRUE(src == var);
			}

			TEST(ADOCastTest, CastTimeSpan)
			{
				_variant_t src(COleDateTimeSpan(2, 12, 10, 33), VT_DATE);
				TimeSpan dst = database_cast<TimeSpan>(src);

				ASSERT_EQ(2, dst.Days());
				ASSERT_EQ(12, dst.Hours());
				ASSERT_EQ(10, dst.Minutes());
				ASSERT_EQ(33, dst.Seconds());

				_variant_t var = database_cast<_variant_t>(dst);
				ASSERT_TRUE(src == var);
			}

			TEST(ADOCastTest, CastTimeSpan2)
			{
				TimeSpan src(-2, -12, -10, -33);

				_variant_t var = database_cast<_variant_t>(src);
				TimeSpan dst = database_cast<TimeSpan>(var);
				ASSERT_TRUE(src == dst);

				EXPECT_EQ(-2, dst.Days());
				EXPECT_EQ(-12, dst.Hours());
				EXPECT_EQ(-10, dst.Minutes());
				EXPECT_EQ(-33, dst.Seconds());
			}

		} // ! Test
	} // ! ADO
} // ! Earlgrey