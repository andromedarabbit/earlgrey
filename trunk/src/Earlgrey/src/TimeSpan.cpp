#include "stdafx.h"
#include "TimeSpan.h"

namespace Earlgrey
{
	const TimeSpan::TickType TimeSpan::ZeroTick = 0L;
	const TimeSpan::TickType TimeSpan::MinTicks = 9223372036854775808L;
	const TimeSpan::TickType TimeSpan::MaxTicks = 0x7fffffffffffffffL;
	const TimeSpan TimeSpan::Zero(ZeroTick);
	const TimeSpan TimeSpan::MinValue(MinTicks);
	const TimeSpan TimeSpan::MaxValue(MaxTicks);

	const double TimeSpan::MillisecondsPerTick = 0.0001;		
	const double TimeSpan::SecondsPerTick = 1E-07;
	const double TimeSpan::MinutesPerTick = 1.6666666666666667E-09;		
	const double TimeSpan::HoursPerTick = 2.7777777777777777E-11;
	const double TimeSpan::DaysPerTick = 1.1574074074074074E-12;
	const int TimeSpan::MillisPerSecond = 0x3e8;
	const int TimeSpan::MillisPerMinute = 0xea60;
	const int TimeSpan::MillisPerHour = 0x36ee80;
	const int TimeSpan::MillisPerDay = 0x5265c00;
	const INT64 TimeSpan::MaxSeconds = 0xd6bf94d5e5L;
	const INT64 TimeSpan::MinSeconds = -922337203685L;
	const INT64 TimeSpan::MaxMilliSeconds = 0x346dc5d638865L;
	const INT64 TimeSpan::MinMilliSeconds = -922337203685477L;

	const TimeSpan::TickType TimeSpan::TicksPerDay =  0xc92a69c000L;
	const TimeSpan::TickType TimeSpan::TicksPerHour = 0x861c46800L; 
	const TimeSpan::TickType TimeSpan::TicksPerMinute = 0x23c34600L; 
	const TimeSpan::TickType TimeSpan::TicksPerSecond = 0x989680L;
	const TimeSpan::TickType TimeSpan::TicksPerMillisecond = 0x2710L;

	TimeSpan TimeSpan::Add(const TimeSpan& ts) const
	{
		TickType ticks = this->m_Ticks + ts.m_Ticks;
		if (((this->m_Ticks >> 0x3f) == (ts.m_Ticks >> 0x3f)) && ((this->m_Ticks >> 0x3f) != (ticks >> 0x3f)))
		{
			// throw new OverflowException(Environment.GetResourceString("Overflow_TimeSpanTooLong"));
		}
		return TimeSpan(ticks);
	}

	TimeSpan TimeSpan::Subtract(const TimeSpan& ts) const
	{
		TickType ticks = this->m_Ticks - ts.m_Ticks;
		if (((this->m_Ticks >> 0x3f) != (ts.m_Ticks >> 0x3f)) && ((this->m_Ticks >> 0x3f) != (ticks >> 0x3f)))
		{
			// throw new OverflowException(Environment.GetResourceString("Overflow_TimeSpanTooLong"));
		}
		return TimeSpan(ticks);
	}



	TimeSpan TimeSpan::Interval(double value, int scale)
	{
		/*
		if (double.IsNaN(value))
		{
		// throw new ArgumentException(Environment.GetResourceString("Arg_CannotBeNaN"));
		}
		*/
		double num = value * scale;
		double num2 = num + ((value >= 0.0) ? 0.5 : -0.5);
		if ((num2 > 922337203685477) || (num2 < -922337203685477))
		{
			// throw new OverflowException(Environment.GetResourceString("Overflow_TimeSpanTooLong"));
		}
		return TimeSpan(((TickType) num2) * 0x2710L);
	}

	TimeSpan::TickType TimeSpan::TimeToTicks(int hour, int minute, int second)
	{
		TickType num = ((hour * 0xe10L) + (minute * 60L)) + second;
		if ((num > MaxSeconds) || (num < -MinSeconds))
		{
			// throw new ArgumentOutOfRangeException(null, Environment.GetResourceString("Overflow_TimeSpanTooLong"));
		}
		return (num * TicksPerSecond);
	}

	TimeSpan::TickType TimeSpan::TimeToTicks(int days, int hours, int minutes, int seconds, int milliseconds)
	{
		TickType num = ((((((days * 0xe10L) * 0x18L) + (hours * 0xe10L)) + (minutes * 60L)) + seconds) * MillisPerSecond) + milliseconds;
		if ((num > MaxMilliSeconds) || (num < -MinMilliSeconds))
		{
			// throw new ArgumentOutOfRangeException(null, Environment.GetResourceString("Overflow_TimeSpanTooLong"));
		}
		return (num * TicksPerMillisecond);
	}

}