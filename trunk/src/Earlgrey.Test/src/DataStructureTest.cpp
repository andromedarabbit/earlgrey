#include "stdafx.h"
#include "EarlgreyAssert.h"
#include "Array.h"

namespace Earlgrey { namespace Test {

	TEST(DataStructureTest, ArrayTest)
	{
		typedef Array<int, 10> MyArray;
		MyArray intArray;
		for (int i=0; i < 10; i++)
		{
			intArray[i] = i;
		}

		EXPECT_TRUE( intArray[10] == MyArray::OutOfRange() );
		EXPECT_TRUE( intArray[8] != MyArray::OutOfRange() );
		EXPECT_TRUE( intArray[5] == 5 );
	}
}}