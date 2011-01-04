#include "stdafx.h"
#include "DateTime.h"
#include "DayOfWeek.h"
#include "DateTimeKind.h"
#include "TimeSpan.h"

#include "txsstream.h"

#if (_MSC_VER >= 1600)
#pragma warning(push)
#pragma warning(disable: 4996)
#endif
#include <iomanip>
#if (_MSC_VER >= 1600)
#pragma warning(pop)
#endif

namespace Earlgrey
{
	const INT64 DateTime::MinTicks = 0L;
	const INT64 DateTime::MaxTicks = 0x2bca2875f4373fffL;
	const DateTime DateTime::MinValue(MinTicks, DATETIMEKIND_UNSPECIFIED);
	const DateTime DateTime::MaxValue(MaxTicks, DATETIMEKIND_UNSPECIFIED);

	const int DateTime::DaysToMonth365[] = { 0, 0x1f, 0x3b, 90, 120, 0x97, 0xb5, 0xd4, 0xf3, 0x111, 0x130, 0x14e, 0x16d };
	const int DateTime::DaysToMonth366[] = { 0, 0x1f, 60, 0x5b, 0x79, 0x98, 0xb6, 0xd5, 0xf4, 0x112, 0x131, 0x14f, 0x16e };

	const INT64 DateTime::TicksPerDay = 0xc92a69c000L;
	const INT64 DateTime::TicksPerHour = 0x861c46800L;
	const INT64 DateTime::TicksPerMillisecond = 0x2710L;
	const INT64 DateTime::TicksPerMinute = 0x23c34600L;
	const INT64 DateTime::TicksPerSecond = 0x989680L;

	DateTime::DateTime(INT64 ticks, E_DateTimeKind kind)
	{
		if ((ticks < MinTicks) || (ticks > MaxTicks))
		{
			// throw new ArgumentOutOfRangeException("ticks", Environment.GetResourceString("ArgumentOutOfRange_DateTimeBadTicks"));
			throw std::exception("");
		}
		if ((kind < DATETIMEKIND_UNSPECIFIED) || (kind > DATETIMEKIND_LOCAL))
		{
			// throw new ArgumentException(Environment.GetResourceString("Argument_InvalidDateTimeKind"), "kind");
			throw std::exception("");
		}
		this->dateData = (UINT64) (ticks | (((INT64) kind) << 0x3e));
	}

	DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, E_DateTimeKind kind)
	{
		INT64 num = DateToTicks(year, month, day) + TimeToTicks(hour, minute, second);
		if ((millisecond < 0) || (millisecond >= 0x3e8))
		{
			// throw new ArgumentOutOfRangeException("millisecond", string.Format(CultureInfo.CurrentCulture, Environment.GetResourceString("ArgumentOutOfRange_Range"), new object[] { 0, 0x3e7 }));
		}
		num += millisecond * TicksPerMillisecond;
		if ((num < 0L) || (num > 0x2bca2875f4373fffL))
		{
			// throw new ArgumentException(Environment.GetResourceString("Arg_DateTimeRange"));
			throw std::exception("");
		}
		if ((kind < DATETIMEKIND_UNSPECIFIED) || (kind > DATETIMEKIND_LOCAL))
		{
			// throw new ArgumentException(Environment.GetResourceString("Argument_InvalidDateTimeKind"), "kind");
			throw std::exception("");
		}
		this->dateData = (UINT64) (num | (((INT64) kind) << 0x3e));
	}

	DateTime DateTime::Add(const TimeSpan& interval) const
	{
		return this->AddTicks(interval.m_Ticks);
	}

	DateTime DateTime::AddTicks(const INT64 ticks) const 
	{
		INT64 internalTicks = this->InternalTicks();
		if ((ticks > (0x2bca2875f4373fffL - internalTicks)) || (ticks < -internalTicks))
		{
			// throw new ArgumentOutOfRangeException("value", Environment.GetResourceString("ArgumentOutOfRange_DateArithmetic"));
			throw std::exception("");
		}
		return DateTime(((UINT64) (internalTicks + ticks)) | this->InternalKind());
	}

	TimeSpan DateTime::TimeOfDay() const
	{
		return TimeSpan(this->InternalTicks() % TicksPerDay);
	}

	//! \todo C#의 구현이 복잡하기 때문에 일단 간단하게 처리한다.
	DateTime DateTime::Now()
	{
		SYSTEMTIME localSystemTime;
		::GetLocalTime(&localSystemTime);

		FILETIME fileTime;
		::SystemTimeToFileTime(&localSystemTime, &fileTime);

		return FromFileTime(fileTime);
	}

	DateTime DateTime::UtcNow() 
	{
		FILETIME fileTime;
		GetSystemTimeAsFileTime(&fileTime);

		return FromFileTime(fileTime);
	}

	//! \ref http://blogs.msdn.com/oldnewthing/archive/2004/08/25/220195.aspx
	DateTime DateTime::FromFileTime(const FILETIME& fileTime)
	{		
// 		UINT64 now;
// 		now |= fileTime.dwHighDateTime;
// 		now <<= 32;
// 		now |= fileTime.dwLowDateTime;

		// UINT64 now = *reinterpret_cast<const UINT64*>(&fileTime);
		ULARGE_INTEGER temp;
		temp.HighPart = fileTime.dwHighDateTime;
		temp.LowPart = fileTime.dwLowDateTime; 
		const UINT64 now = temp.QuadPart;

		// return new DateTime((UINT64) ((GetSystemTimeAsFileTime() + 0x701ce1722770000L) | 0x4000000000000000L));
		return DateTime((UINT64) ((now + 0x701ce1722770000L) | 0x4000000000000000L));
	}

	E_DateTimeKind DateTime::Kind() const
	{
		switch (this->InternalKind())
		{
		case 0L:
			return DATETIMEKIND_UNSPECIFIED;

		case 0x4000000000000000L:
			return DATETIMEKIND_UTC;
		}
		return DATETIMEKIND_LOCAL;
	}

	E_DayOfWeek DateTime::DayOfWeek() const
	{
		return static_cast<E_DayOfWeek>(
			// ((int) (((this->InternalTicks() / TicksPerDay) + 1L) % 7L))
			static_cast<int>(
				(((this->InternalTicks() / TicksPerDay) + 1L) % 7L)
				)
			);
	}

	int DateTime::DaysInMonth(int year, int month)
	{
		if ((month < 1) || (month > 12))
		{
			// throw new ArgumentOutOfRangeException("month", Environment.GetResourceString("ArgumentOutOfRange_Month"));
			throw std::exception("");
		}
		const int* numArray = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
		return (numArray[month] - numArray[month - 1]);
	}

	DateTime DateTime::Add(double thisValue, int scale) const
	{
		INT64 num = (INT64) ((thisValue * scale) + ((thisValue >= 0.0) ? 0.5 : -0.5));
		if ((num <= -315537897600000L) || (num >= 0x11efae44cb400L))
		{
			throw std::exception("");
			// throw new ArgumentOutOfRangeException("value", Environment.GetResourceString("ArgumentOutOfRange_AddValue"));
		}
		return this->AddTicks(num * TicksPerMillisecond);
	}

	DateTime DateTime::AddMonths(int months) const
	{
		if ((months < -120000) || (months > 0x1d4c0))
		{
			// throw new ArgumentOutOfRangeException("months", Environment.GetResourceString("ArgumentOutOfRange_DateTimeBadMonths"));
		}
		int datePart = this->GetDatePart(0);
		int month = this->GetDatePart(2);
		int day = this->GetDatePart(3);
		int num4 = (month - 1) + months;
		if (num4 >= 0)
		{
			month = (num4 % 12) + 1;
			datePart += num4 / 12;
		}
		else
		{
			month = 12 + ((num4 + 1) % 12);
			datePart += (num4 - 11) / 12;
		}
		if ((datePart < 1) || (datePart > 0x270f))
		{
			// throw new ArgumentOutOfRangeException("months", Environment.GetResourceString("ArgumentOutOfRange_DateArithmetic"));
			throw std::exception("");
		}
		int num5 = DaysInMonth(datePart, month);
		if (day > num5)
		{
			day = num5;
		}
		return DateTime(((UINT64) (DateToTicks(datePart, month, day) + (this->InternalTicks() % TicksPerDay))) | this->InternalKind());
	}

	TimeSpan DateTime::Subtract(const DateTime& interval) const
	{
		return TimeSpan(this->InternalTicks() - interval.InternalTicks());
	}

	DateTime DateTime::Subtract(const TimeSpan& interval) const
	{
		INT64 internalTicks = this->InternalTicks();
		INT64 num2 = interval.m_Ticks;
		if ((internalTicks < num2) || ((internalTicks - 0x2bca2875f4373fffL) > num2))
		{
			// throw new ArgumentOutOfRangeException("value", Environment.GetResourceString("ArgumentOutOfRange_DateArithmetic"));
			throw std::exception("");
		}
		return DateTime(((UINT64) (internalTicks - num2)) | this->InternalKind());
	}

	DateTime DateTime::operator + (const TimeSpan& other)
	{
		return Add(other);
	}

	DateTime DateTime::operator + (const TimeSpan& other) const
	{
		return Add(other);
	}

	DateTime DateTime::operator - (const TimeSpan& other)
	{
		return Subtract(other);
	}

	DateTime DateTime::operator - (const TimeSpan& other) const
	{
		return Subtract(other);
	}

	TimeSpan DateTime::operator - (const DateTime& other)
	{
		return TimeSpan(this->InternalTicks() - other.InternalTicks());

	}

	TimeSpan DateTime::operator - (const DateTime& other) const
	{
		return TimeSpan(this->InternalTicks() - other.InternalTicks());
	}

	int DateTime::GetDatePart(int part) const
	{
		int num2 = (int) (this->InternalTicks() / TicksPerDay);
		int num3 = num2 / 0x23ab1;
		num2 -= num3 * 0x23ab1;
		int num4 = num2 / 0x8eac;
		if (num4 == 4)
		{
			num4 = 3;
		}
		num2 -= num4 * 0x8eac;
		int num5 = num2 / 0x5b5;
		num2 -= num5 * 0x5b5;
		int num6 = num2 / 0x16d;
		if (num6 == 4)
		{
			num6 = 3;
		}
		if (part == 0)
		{
			return (((((num3 * 400) + (num4 * 100)) + (num5 * 4)) + num6) + 1);
		}
		num2 -= num6 * 0x16d;
		if (part == 1)
		{
			return (num2 + 1);
		}
		const int* numArray = ((num6 == 3) && ((num5 != 0x18) || (num4 == 3))) ? DaysToMonth366 : DaysToMonth365;
		int index = num2 >> 6;
		while (num2 >= numArray[index])
		{
			index++;
		}
		if (part == 2)
		{
			return index;
		}
		return ((num2 - numArray[index - 1]) + 1);
	}


	BOOL DateTime::IsLeapYear(int year)
	{
		if ((year < 1) || (year > 0x270f))
		{
			// throw new ArgumentOutOfRangeException("year", Environment.GetResourceString("ArgumentOutOfRange_Year"));
			throw std::exception("");
		}
		if ((year % 4) != 0)
		{
			return FALSE;
		}
		if ((year % 100) == 0)
		{
			return ((year % 400) == 0);
		}
		return TRUE;
	}

	INT64 DateTime::DateToTicks(int year, int month, int day)
	{
		if (((year >= 1) && (year <= 0x270f)) && ((month >= 1) && (month <= 12)))
		{
			const int* numArray = IsLeapYear(year) ? DaysToMonth366 : DaysToMonth365;
			if ((day >= 1) && (day <= (numArray[month] - numArray[month - 1])))
			{
				int num = year - 1;
				int num2 = ((((((num * 0x16d) + (num / 4)) - (num / 100)) + (num / 400)) + numArray[month - 1]) + day) - 1;
				return (num2 * TicksPerDay);
			}
		}

		// TODO: 임시
		// throw new ArgumentOutOfRangeException(null, Environment.GetResourceString("ArgumentOutOfRange_BadYearMonthDay"));
		throw std::exception("");
		// throw 1;
	}

	INT64 DateTime::TimeToTicks(int hour, int minute, int second)
	{
		if ((((hour < 0) || (hour >= 0x18)) || ((minute < 0) || (minute >= 60))) || ((second < 0) || (second >= 60)))
		{
			// throw new ArgumentOutOfRangeException(null, Environment.GetResourceString("ArgumentOutOfRange_BadHourMinuteSecond"));
			throw std::exception("");
		}
		return TimeSpan::TimeToTicks(hour, minute, second);
	}

	_txstring DateTime::ToString() const
	{
		// _tcsftime() 와 struct tm의 조합은 밀리초 단위까지 표현하지 못하므로 쓰지 않는다.
		_txostringstream ss;
		ss << Year()
			<< _T("-") << std::setfill(_T('0')) << std::setw(2) << Month()
			<< _T("-") << std::setfill(_T('0')) << std::setw(2) << Day()
			<< _T(" ") << std::setfill(_T('0')) << std::setw(2) << Hour()
			<< _T(":") << std::setfill(_T('0')) << std::setw(2) << Minute()
			<< _T(":") << std::setfill(_T('0')) << std::setw(2) << Second()
			<< _T(".") << std::setfill(_T('0')) << std::setw(6) << Millisecond()
			;

		return ss.str();
	}

	_tostream& operator << (_tostream &os, const DateTime &obj)
	{
		os << obj.ToString();
		return os;
	}
}
