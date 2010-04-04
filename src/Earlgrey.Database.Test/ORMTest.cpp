#include "stdafx.h"
#include "ORMTest.h"

#include "ADOEnvironmentTest.h"

#include "Uncopyable.h"
#include "tiostream.h"



namespace Earlgrey {
namespace ADO {
namespace Test {

	class ORDTest : public ADOEnvironmentTest
	{

	};


	TEST(ORDTest, ColumnEqualOperator)
	{
		Column<INT32> integerColumn;
		ASSERT_TRUE(integerColumn == 0);
		ASSERT_TRUE(0 == integerColumn);

// 		const TCHAR * hello = _T("hello");
// 		Column<_txstring> stringColumn(hello);
// 		ASSERT_TRUE(stringColumn == hello);
// 		ASSERT_TRUE(hello == stringColumn);
	}

	TEST(ORDTest, ColumnNotEqualOperator)
	{
		Column<INT32> integerColumn(123);
		ASSERT_TRUE(integerColumn != 0);
		ASSERT_TRUE(0 != integerColumn);
	}

	TEST(ORDTest, ColumnGreaterOperator)
	{
		Column<INT32> integerColumn(123);
		ASSERT_TRUE(integerColumn > 0);
	}

	TEST(ORDTest, ColumnGreaterThanOperator)
	{
		Column<INT32> integerColumn(123);
		ASSERT_TRUE(integerColumn >= 122);
		ASSERT_TRUE(integerColumn >= 123);
	}

	TEST(ORDTest, ColumnLessOperator)
	{
		Column<INT32> integerColumn(123);
		ASSERT_TRUE(integerColumn < 123456);
	}

	TEST(ORDTest, ColumnLessThanOperator)
	{
		Column<INT32> integerColumn(123);
		ASSERT_TRUE(integerColumn <= 123456);
		ASSERT_TRUE(integerColumn <= 123);
	}



}
}
}