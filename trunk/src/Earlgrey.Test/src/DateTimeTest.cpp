#include "stdafx.h"
#include "TimeSpan.h"
#include "DateTime.h"
#include "DateTimeKind.h"

#include "txsstream.h"

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

		TEST(DateTimeTest, StreamOutput)
		{
			DateTime dateTime(2009, 4, 2, 11, 39, 10, 213, DATETIMEKIND_LOCAL);

			_txostringstream ss;
			ss << dateTime;

			_txstring str = ss.str();
			ASSERT_TRUE(str == _T("2009-04-02 11:39:10.000213"));
		}

		TEST(DateTimeTest, Now)
		{
			SYSTEMTIME localSystemTime;
			::GetLocalTime(&localSystemTime);

			DateTime now = DateTime::Now();
			ASSERT_EQ(localSystemTime.wYear, now.Year());
			ASSERT_EQ(localSystemTime.wMonth, now.Month());
			ASSERT_EQ(localSystemTime.wDay, now.Day());
			ASSERT_EQ(localSystemTime.wHour, now.Hour());
			ASSERT_EQ(localSystemTime.wMinute, now.Minute());
			ASSERT_EQ(localSystemTime.wSecond, now.Second());
			ASSERT_EQ(localSystemTime.wMilliseconds, now.Millisecond());

		}
	}
}