#pragma once 
#include "tiostream.h"
#include "txstring.h"

namespace Earlgrey
{
	class TimeSpan;		 
	enum E_DayOfWeek;
	enum E_DateTimeKind;

	class DateTime
	{
		friend _tostream& operator << (_tostream& stream, const DateTime& obj);
		

	public:
		static const INT64 MaxTicks;
		static const INT64 MinTicks;
		static const DateTime MaxValue;
		static const DateTime MinValue;		


		explicit DateTime(INT64 ticks)
		{
			if ((ticks < MinTicks) || (ticks > MaxTicks))
			{
				// throw new ArgumentOutOfRangeException("ticks", Environment.GetResourceString("ArgumentOutOfRange_DateTimeBadTicks"));
			}
			this->dateData = static_cast<UINT64>( ticks );
		}

		explicit DateTime(const UINT64 dateData)
		{
			this->dateData = dateData;
		}

		explicit DateTime(INT64 ticks, E_DateTimeKind kind);

		explicit DateTime(int year, int month, int day)
		{
			this->dateData = static_cast<UINT64>( DateToTicks(year, month, day) );
		}

		explicit DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, E_DateTimeKind kind);


		DateTime Add(const TimeSpan& interval) const;

		inline DateTime AddDays(double days) const
		{
			return this->Add(days, 0x5265c00);
		}

		inline DateTime AddHours(double hours) const
		{
			return this->Add(hours, 0x36ee80);
		}

		inline DateTime AddMilliseconds(double milliseconds) const
		{
			return this->Add(milliseconds, 1);
		}

		inline DateTime AddMinutes(double minutes) const
		{
			return this->Add(minutes, 0xea60);
		}

		DateTime AddMonths(int months) const;
		
		inline DateTime AddSeconds(double seconds) const
		{
			return this->Add(seconds, 0x3e8);
		}


		DateTime AddTicks(const INT64 ticks) const;

		inline DateTime AddYears(int years) const
		{
			if ((years < -10000) || (years > 0x2710))
			{
				// throw new ArgumentOutOfRangeException("years", Environment.GetResourceString("ArgumentOutOfRange_DateTimeBadYears"));
			}
			return this->AddMonths(years * 12);
		}

		TimeSpan Subtract(const DateTime& interval) const;

		DateTime Subtract(const TimeSpan& interval) const;

		DateTime operator + (const TimeSpan& other);

		DateTime operator + (const TimeSpan& other) const;

		DateTime operator - (const TimeSpan& other);

		DateTime operator - (const TimeSpan& other) const;

		TimeSpan operator - (const DateTime& other);

		TimeSpan operator - (const DateTime& other) const;

		inline bool operator == (const DateTime& other) const
		{
			return this->InternalTicks() == other.InternalTicks();
		}

		inline bool operator != (const DateTime& other) const
		{
			return this->InternalTicks() != other.InternalTicks();
		}

		inline bool operator <= (const DateTime& other) const
		{
			return this->InternalTicks() <= other.InternalTicks();	
		}

		inline bool operator >= (const DateTime& other) const
		{
			return this->InternalTicks() >= other.InternalTicks();
		}

		inline bool operator < (const DateTime& other) const
		{
			return this->InternalTicks() < other.InternalTicks();
		}

		inline bool operator > (const DateTime& other) const
		{
			return this->InternalTicks() > other.InternalTicks();
		}


		inline DateTime Date() const
		{	
			UINT64 internalTicks = this->InternalTicks();
			return DateTime(((UINT64) (internalTicks - (internalTicks % TicksPerDay))) | this->InternalKind());
		}
		
		inline int Day() const
		{
			return this->GetDatePart(3);
		}

		E_DayOfWeek DayOfWeek() const;

		inline int DayOfYear() const
		{
			return this->GetDatePart(1);
		}

		inline int Hour() const
		{
			return static_cast<int>( 
				((this->InternalTicks() / TicksPerHour) % 0x18L)
				);
		}

		E_DateTimeKind Kind() const;

		inline int Millisecond() const
		{
			return static_cast<int>(
				((this->InternalTicks() / TicksPerMillisecond) % 0x3e8L)
				);
		}

		inline int Minute() const
		{
			return static_cast<int>(
				((this->InternalTicks() / TicksPerMinute) % 60L)
				);
		}

		inline int Month() const
		{
			return this->GetDatePart(2);
		}


		static DateTime Now();

		static DateTime UtcNow();

		inline int Second() const
		{
			return static_cast<int>(
				((this->InternalTicks() / TicksPerSecond) % 60L)
				);
		}

		inline INT64 Ticks() const
		{
			return this->InternalTicks();
		}
		
		inline TimeSpan TimeOfDay() const;

		/*
		static DateTime Today
		{
			return Now.Date;
		}
		*/

		inline int Year() const
		{
			return this->GetDatePart(0);
		}


		inline int CompareTo(const DateTime& other) const
		{
			INT64 internalTicks = other.InternalTicks();
			INT64 num2 = this->InternalTicks();
			if (num2 > internalTicks)
			{
				return 1;
			}
			if (num2 < internalTicks)
			{
				return -1;
			}
			return 0;
		}


		_txstring ToString() const;

	private:
		static DateTime FromFileTime(const FILETIME& fileTime);

		static BOOL IsLeapYear(int year);

		static INT64 DateToTicks(int year, int month, int day);

		static INT64 TimeToTicks(int hour, int minute, int second);

		static int DaysInMonth(int year, int month);
		

		DateTime Add(double thisValue, int scale) const;


		int GetDatePart(int part) const;		


		inline INT64 InternalTicks() const
		{
			return (((INT64) this->dateData) & 0x3fffffffffffffffL);
		}

		inline UINT64 InternalKind() const
		{
			return (this->dateData & 13835058055282163712L);
		}

	private:
		static const int DaysToMonth365[];
		static const int DaysToMonth366[];

		static const INT64 TicksPerDay;
		static const INT64 TicksPerHour;
		static const INT64 TicksPerMillisecond;
		static const INT64 TicksPerMinute;
		static const INT64 TicksPerSecond;


		UINT64 dateData;

	};

	_tostream& operator << (_tostream &os, const DateTime &obj);
}