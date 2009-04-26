#include "stdafx.h"
#include "DateTime.h"
#include "DateTimeKind.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(DateTimeTest, LongestConstructor)
		{
			DateTime dateTime(2009, 4, 2, 11, 33, 44, 55, DATETIMEKIND_LOCAL);

			ASSERT_EQ(2009, dateTime.Year());
			ASSERT_EQ(4, dateTime.Month());
			ASSERT_EQ(2, dateTime.Day());
			ASSERT_EQ(11, dateTime.Hour());
			ASSERT_EQ(33, dateTime.Minute());
			ASSERT_EQ(44, dateTime.Second());
			ASSERT_EQ(55, dateTime.Millisecond());			
		}


	}
}