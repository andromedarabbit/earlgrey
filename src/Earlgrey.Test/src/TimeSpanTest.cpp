#include "stdafx.h"
#include "TimeSpan.h"
#include "StlCustom.h"

namespace Earlgrey
{
	namespace Test
	{
		namespace 
		{
			static const TimeSpan::TickType MILLISECOND_TICKS = 10000; 
			static const TimeSpan::TickType SECOND_TICKS = MILLISECOND_TICKS * 1000; 
			static const TimeSpan::TickType MINUTE_TICKS = MILLISECOND_TICKS * 1000 * 60; 
		}

		TEST(TimeSpanTest, OneMillisecondAndOneTick)
		{
			TimeSpan interval(MILLISECOND_TICKS + 1);

			ASSERT_EQ( 1.0001, interval.TotalMilliseconds() );
		}

		TEST(TimeSpanTest, OneSecond)
		{
			TimeSpan interval(SECOND_TICKS);

			ASSERT_EQ( 1000, interval.TotalMilliseconds() );			
			ASSERT_EQ( 1, interval.TotalSeconds() );
			ASSERT_DOUBLE_EQ( 0.01666666666666666, interval.TotalMinutes() );
		}

		TEST(TimeSpanTest, EqualOperator)
		{
			TimeSpan interval1(SECOND_TICKS);
			TimeSpan interval2(SECOND_TICKS);

			ASSERT_TRUE( interval1 == interval2 );			
			ASSERT_FALSE( interval1 != interval2 );
		}

		TEST(TimeSpanTest, PlusOperator)
		{
			TimeSpan oneMinute(MINUTE_TICKS);
			TimeSpan twoMinute(MINUTE_TICKS * 2);

			TimeSpan sum = oneMinute + twoMinute;
			
			ASSERT_EQ( 3, static_cast<int>(sum.TotalMinutes()) );
		}

		TEST(TimeSpanTest, MinusOperator)
		{
			TimeSpan oneMinute(MINUTE_TICKS);
			TimeSpan twoMinute(MINUTE_TICKS * 2);

			TimeSpan sum = twoMinute - oneMinute;

			ASSERT_EQ( 1, static_cast<int>(sum.TotalMinutes()) );
		}

		TEST(TimeSpanTest, ToStringWithNoMilliseconds)
		{
			TimeSpan interval(MINUTE_TICKS + SECOND_TICKS);
			_txstring str = interval.ToString();

			ASSERT_STREQ( TEXT("00:01:01"), str.c_str() );
		}

		TEST(TimeSpanTest, ToString)
		{
			TimeSpan interval(MINUTE_TICKS + SECOND_TICKS + MILLISECOND_TICKS);
			_txstring str = interval.ToString();

			ASSERT_STREQ( TEXT("00:01:01.0010000"), str.c_str() );

		}
	}
}