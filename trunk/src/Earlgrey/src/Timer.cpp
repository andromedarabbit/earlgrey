#include "stdafx.h"
#include "Timer.h"


namespace Earlgrey
{
	Timer::Timer(TimerCallbackPtr callback, void * state, TimeSpan dueTime, TimeSpan period)
		: m_callback(callback)
		, m_state(state)
		, m_dueTime(dueTime)
		, m_period(period)
	{

	}

	Timer::Timer(TimerCallbackPtr callback)
		: m_callback(callback)
		, m_state(NULL)
		, m_dueTime(TimeSpan::MaxValue)
		, m_period(TimeSpan::MaxValue)
	{

	}

	void Timer::Change(TimeSpan dueTime, TimeSpan period)
	{
		m_dueTime = dueTime;
		m_period = period;
	}

	void Timer::Close()
	{

	}

}
