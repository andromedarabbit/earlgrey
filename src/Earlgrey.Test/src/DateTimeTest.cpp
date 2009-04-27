#include "stdafx.h"
#include "TimeSpan.h"
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

		TEST(DateTimeTest, DaysBetweenDates)
		{
			DateTime dateTime1(2009, 4, 2);
			DateTime dateTime2(2009, 4, 5);

			TimeSpan interval = dateTime2 - dateTime1;
			ASSERT_EQ(3, interval.TotalDays());
		}

		TEST(DateTimeTest, DateTimePlusTimeSpan)
		{
			DateTime dateTime(2009, 4, 2);
			TimeSpan interval = TimeSpan::FromHours(24);

			DateTime sum = dateTime + interval;
			DateTime expected(2009, 4, 3);
			ASSERT_EQ(expected.Ticks(), sum.Ticks());
		}
	}
}