#pragma once 

namespace Earlgrey
{	
	//! \ref http://kaistizen.net/EE/index.php/imaso/cplusplus_cli_lecture_2009_01/
	class TimeSpan
	{
		friend class NDateTime;
	public:
		typedef signed __int64 TickType;

		static const TickType TicksPerDay =  864000000000; // 24 * 60 * 60 * 1000 * 1000 * 10
		static const TickType TicksPerHour = 36000000000; // 60 * 60 * 1000 * 1000 * 10
		static const TickType TicksPerMinute = 600000000; 
		static const TickType TicksPerSecond = 10000000;
		static const TickType TicksPerMillisecond = 10000;

		TimeSpan()
			: m_Time(0)
		{
		}

		TimeSpan(const TickType ticks)
			: m_Time(ticks)
		{
		}

		TimeSpan(const TimeSpan& other)
			: m_Time(other.m_Time)
		{
		}

		inline TickType Ticks() const
		{
			return m_Time;
		}

		inline int Days() const
		{
			return static_cast<int> (m_Time / TicksPerDay);
		}

		inline int Hours() const
		{
			return static_cast<int> ((m_Time / TicksPerHour) % 24L);
		}

		inline int Minutes() const
		{
			return static_cast<int> ((m_Time / TicksPerMinute) % 60L);
		}

		inline int Seconds() const
		{
			return static_cast<int> ((m_Time / TicksPerSecond) % 60L);
		}

		inline int Milliseconds() const
		{
			return static_cast<int> ((m_Time / TicksPerMillisecond) % 1000L);;
		}

		inline double TotalDays() const
		{
			return (m_Time * 1.15740740740741E-12);
		}

		inline double TotalHours() const
		{
			return (m_Time * 2.77777777777778E-11);
		}

		inline double TotalMinutes() const
		{
			return (m_Time * 1.66666666666667E-09);
		}

		inline double TotalSeconds() const
		{
			return (m_Time * 1E-07);
		}

		inline double TotalMilliseconds() const
		{
			double num = m_Time * 0.0001;
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

		TimeSpan operator+(const TimeSpan& other)
		{
			return TimeSpan(this->m_Time + other.m_Time);
		}

		TimeSpan operator+(const TimeSpan& other) const
		{
			return TimeSpan(this->m_Time + other.m_Time);
		}

		TimeSpan operator-(const TimeSpan& other)
		{
			return TimeSpan(this->m_Time - other.m_Time);
		}

		TimeSpan operator-(const TimeSpan& other) const
		{
			return TimeSpan(this->m_Time - other.m_Time);
		}

		bool operator==(const TimeSpan& other) const
		{
			return this->m_Time == other.m_Time;
		}

		bool operator==(const TickType otherTick) const
		{
			return this->m_Time == otherTick;
		}

		bool operator!=(const TimeSpan& other) const
		{
			return this->m_Time != other.m_Time;
		}

		bool operator!=(const TickType otherTick) const
		{
			return this->m_Time != otherTick;
		}

		bool operator<=(const TimeSpan& other) const
		{
			return this->m_Time <= other.m_Time;	
		}

		bool operator<=(const TickType otherTick) const
		{
			return this->m_Time <= otherTick;	
		}

		bool operator>=(const TimeSpan& other) const
		{
			return this->m_Time >= other.m_Time;
		}

		bool operator<(const TimeSpan& other) const
		{
			return this->m_Time < other.m_Time;
		}

		bool operator>(const TimeSpan& other) const
		{
			return this->m_Time > other.m_Time;
		}

		bool operator<(const TickType otherTick) const
		{
			return this->m_Time < otherTick;
		}

		bool operator>(const TickType otherTick) const
		{
			return this->m_Time > otherTick;
		}

		operator TickType() const
		{
			return m_Time;
		}

	private:
		TickType m_Time;
	};

}