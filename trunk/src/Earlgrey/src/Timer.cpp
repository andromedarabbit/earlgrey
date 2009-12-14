#include "stdafx.h"
#include "Timer.h"


namespace Earlgrey
{
	using namespace std::tr1;

	/*shared_ptr<Timer> Timer::NewInstance(
		TimerCallbackPtr callback
		, void * state
		, TimeSpan dueTime
		, TimeSpan period
		)
	{
		return shared_ptr<Timer>(new Timer(callback, state, dueTime, period));
	}

	shared_ptr<Timer> Timer::NewInstance(
		TimerCallbackPtr callback
		)
	{
		return shared_ptr<Timer>(new Timer(callback));
	}*/

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

		// this->Run();
	}

	void Timer::Close()
	{

	}

	void Timer::Close(HANDLE waitHandle)
	{
		DBG_UNREFERENCED_PARAMETER(waitHandle);
	}

}
