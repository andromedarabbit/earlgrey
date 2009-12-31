#include "stdafx.h"
#include "Timer.h"
#include "TimerManager.h"

namespace Earlgrey
{
	using namespace std::tr1;

	Timer::Timer(TimerCallback callback, StatePtr state, TimeSpan dueTime, TimeSpan period, ThreadIdType tid)
		: m_callback(callback)
		, m_state(state)
		, m_dueTime(dueTime)
		, m_period(period)
		, m_runnableID(TimerRunnable::INVALID_ID)
		, m_tid(tid)
	{
		RegisterTimer();
	}

	Timer::Timer(TimerCallback callback, ThreadIdType tid)
		: m_callback(callback)
		, m_state()
		, m_dueTime(TimeSpan::MaxValue)
		, m_period(TimeSpan::MaxValue)
		, m_runnableID(TimerRunnable::INVALID_ID)
		, m_tid(tid)
	{
		RegisterTimer();
	}

	void Timer::Change(TimeSpan dueTime, TimeSpan period)
	{
		m_dueTime = dueTime;
		m_period = period;

		RegisterTimer();
	}

	void Timer::Close()
	{
		Close(NULL);
	}

	void Timer::Close(HANDLE waitHandle)
	{
		EARLGREY_ASSERT(m_runnableID != TimerRunnable::INVALID_ID);

		TimerManager::DeregisterRequest task( new TimerDeregisterMessage(m_runnableID, waitHandle) );
		TimerManagerSingleton::Instance().Deregister(task, m_tid);
	}

	void Timer::RegisterTimer()
	{
		if(m_dueTime < TimeSpan::Zero)
			return;

		TimerManager::Task task(
			new TimerRunnable(m_callback, m_state, m_dueTime, m_period) 
			);
		m_runnableID = task->ID();
		TimerManagerSingleton::Instance().Register(task, m_tid);
	}

}
