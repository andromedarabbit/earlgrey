#pragma once 
#include "tiostream.h"
#include "txstring.h"

namespace Earlgrey
{	
	// class _txstring;

	//! \ref http://kaistizen.net/EE/index.php/imaso/cplusplus_cli_lecture_2009_01/
	class TimeSpan
	{
		friend class DateTime;
		// friend _tostream& operator << (_tostream& stream, const TimeSpan& obj);
		// friend _tistream& operator >> (_tistream& stream, TimeSpan& obj);

		friend std::ostream& operator << (std::ostream& stream, const TimeSpan& obj);
		friend std::wostream& operator << (std::wostream& stream, const TimeSpan& obj);

	public:
		typedef INT64 TickType;

		static const TickType ZeroTick;
		static const TickType MaxTicks;
		static const TickType MinTicks;
		static const TimeSpan Zero;
		static const TimeSpan MaxValue;
		static const TimeSpan MinValue;		

		static const TickType TicksPerDay;
		static const TickType TicksPerHour;
		static const TickType TicksPerMinute;
		static const TickType TicksPerSecond;
		static const TickType TicksPerMillisecond;

		explicit TimeSpan()
			: m_Ticks(0)
		{
		}

		explicit TimeSpan(const TickType ticks)
			: m_Ticks(ticks)
		{
		}

		TimeSpan(const TimeSpan& other)
			: m_Ticks(other.m_Ticks)
		{
		}

		explicit TimeSpan(int hours, int minutes, int seconds)
		{
			this->m_Ticks = TimeToTicks(hours, minutes, seconds);
		}

		explicit TimeSpan(int days, int hours, int minutes, int seconds)
		{
			this->m_Ticks = TimeToTicks(days, hours, minutes, seconds, 0);			
		}

		TimeSpan(int days, int hours, int minutes, int seconds, int milliseconds)
		{
			this->m_Ticks = TimeToTicks(days, hours, minutes, seconds, milliseconds);			
		}

		static TimeSpan FromHours(double hours)
		{
			return Interval(hours, MillisPerHour);
		}

		static TimeSpan FromMinutes(double value)
		{
			return Interval(value, MillisPerMinute);
		}

		static TimeSpan FromSeconds(double value)
		{
			return Interval(value, MillisPerSecond);
		}

		static TimeSpan FromMilliseconds(double value)
		{
			return Interval(value, 1);
		}

		inline TickType Ticks() const
		{
			return m_Ticks;
		}

		inline int Days() const
		{
			return static_cast<int> (m_Ticks / TicksPerDay);
		}

		inline int Hours() const
		{
			return static_cast<int> ((m_Ticks / TicksPerHour) % 24L);
		}

		inline int Minutes() const
		{
			return static_cast<int> ((m_Ticks / TicksPerMinute) % 60L);
		}

		inline int Seconds() const
		{
			return static_cast<int> ((m_Ticks / TicksPerSecond) % 60L);
		}

		inline int Milliseconds() const
		{
			return static_cast<int> ((m_Ticks / TicksPerMillisecond) % 1000L);;
		}

		inline double TotalDays() const
		{
			return (m_Ticks * DaysPerTick);
		}

		inline double TotalHours() const
		{
			return (m_Ticks * HoursPerTick);
		}

		inline double TotalMinutes() const
		{
			return (m_Ticks * MinutesPerTick);
		}

		inline double TotalSeconds() const
		{
			return (m_Ticks * SecondsPerTick);
		}

		inline double TotalMilliseconds() const
		{
			double num = m_Ticks * 0.0001;
			if (num > 922337203685477)
			{
				return 922337203685477;
			}
			if (num < -922337203685477)
			{
				return -922337203685477;
			}
			return num;
		}

		TimeSpan Negate() const
		{
			if (this->m_Ticks == MinTicks)
			{
				// throw new OverflowException(Environment.GetResourceString("Overflow_NegateTwosCompNum"));
			}
			return TimeSpan(-this->m_Ticks);
		}

		TimeSpan Add(const TimeSpan& ts) const;

		TimeSpan Subtract(const TimeSpan& ts) const ;

		TimeSpan operator + (const TimeSpan& other)
		{
			return Add(other);
		}

		TimeSpan operator + (const TimeSpan& other) const
		{
			return Add(other);
		}

		TimeSpan operator - (const TimeSpan& other)
		{
			return Subtract(other);
		}

		TimeSpan operator - (const TimeSpan& other) const
		{
			return Subtract(other);
		}

		// friend TimeSpan operator - (const TimeSpan& other);

		bool operator == (const TimeSpan& other) const
		{
			return this->m_Ticks == other.m_Ticks;
		}

		bool operator == (const TickType otherTick) const
		{
			return this->m_Ticks == otherTick;
		}

		bool operator != (const TimeSpan& other) const
		{
			return this->m_Ticks != other.m_Ticks;
		}

		bool operator != (const TickType otherTick) const
		{
			return this->m_Ticks != otherTick;
		}

		bool operator <= (const TimeSpan& other) const
		{
			return this->m_Ticks <= other.m_Ticks;	
		}

		bool operator <= (const TickType otherTick) const
		{
			return this->m_Ticks <= otherTick;	
		}

		bool operator >= (const TimeSpan& other) const
		{
			return this->m_Ticks >= other.m_Ticks;
		}

		bool operator < (const TimeSpan& other) const
		{
			return this->m_Ticks < other.m_Ticks;
		}

		bool operator > (const TimeSpan& other) const
		{
			return this->m_Ticks > other.m_Ticks;
		}

		bool operator < (const TickType otherTick) const
		{
			return this->m_Ticks < otherTick;
		}

		bool operator > (const TickType otherTick) const
		{
			return this->m_Ticks > otherTick;
		}

		int CompareTo(const TimeSpan& other) const
		{
			TickType num = other.m_Ticks;
			if (this->m_Ticks > num)
			{
				return 1;
			}
			if (this->m_Ticks < num)
			{
				return -1;
			}
			return 0;
		}

		_txstring ToString() const;


		private:
			static TimeSpan Interval(double value, int scale);

			static TickType TimeToTicks(int hour, int minute, int second);
			static TickType TimeToTicks(int days, int hours, int minutes, int seconds, int milliseconds);

	private:
		static const double MillisecondsPerTick;
		static const double SecondsPerTick;
		static const double MinutesPerTick;
		static const double HoursPerTick;
		static const double DaysPerTick;
		static const int MillisPerSecond;
		static const int MillisPerMinute;
		static const int MillisPerHour;
		static const int MillisPerDay;
		static const INT64 MaxSeconds;
		static const INT64 MinSeconds;
		static const INT64 MaxMilliSeconds;
		static const INT64 MinMilliSeconds;


		TickType m_Ticks;
	};


	/*
	TimeSpan operator - (const TimeSpan& other)
	{
		if (other.m_Ticks == TimeSpan::MinTicks)
		{
			// throw new OverflowException(Environment.GetResourceString("Overflow_NegateTwosCompNum"));
		}
		return TimeSpan(-other.m_Ticks);
	}
	*/

	// TODO: 임시 코드
	inline std::ostream& operator << (std::ostream& stream, const TimeSpan& obj)
	{
		stream << obj.Ticks() << " ticks";
		return stream;
		// stream << obj.ToString();
	}

	inline std::wostream& operator << (std::wostream& stream, const TimeSpan& obj)
	{
		stream << obj.ToString();
		return stream;
	}
	// _tistream& operator >> (_tistream& stream, TimeSpan& obj);
}

