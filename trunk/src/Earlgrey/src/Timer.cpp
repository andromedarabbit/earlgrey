#include "stdafx.h"
#include "Timer.h"
#include "TimerManager.h"

namespace Earlgrey
{
	using namespace std::tr1;

	Timer::Timer(TimerCallbackPtr callback, StatePtr state, TimeSpan dueTime, TimeSpan period)
		: m_callback(callback)
		, m_state(state)
		, m_dueTime(dueTime)
		, m_period(period)
	{

	}

	Timer::Timer(TimerCallbackPtr callback)
		: m_callback(callback)
		, m_state()
		, m_dueTime(TimeSpan::MaxValue)
		, m_period(TimeSpan::MaxValue)
	{

	}

	void Timer::Change(TimeSpan dueTime, TimeSpan period)
	{
		m_dueTime = dueTime;
		m_period = period;

		// TimerManager::Task task( new TimerRunnable() );
		// TimerManagerSingleton::Instance().Register(task, INVALID_THREAD_ID);
	}

	void Timer::Close()
	{
		// TimerManager::Task task( new TimerRunnable() );
		// TimerManagerSingleton::Instance().Deregister(task, INVALID_THREAD_ID);
	}

	void Timer::Close(HANDLE waitHandle)
	{
		DBG_UNREFERENCED_PARAMETER(waitHandle);
		// TimerManager::Task task( new TimerRunnable(waitHandle) );
		// TimerManagerSingleton::Instance().Deregister(task, INVALID_THREAD_ID);
	}

}
