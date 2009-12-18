#pragma once
#include "Runnable.h"
#include "TimerCallback.h"
#include "DateTime.h"
#include "TimeSpan.h"

namespace Earlgrey
{
	class TimerRunnable : public IRunnable
	{
	public:
		explicit TimerRunnable(TimerCallbackPtr callback, StatePtr state, TimeSpan dueTime,	TimeSpan period)
			: m_startTime(DateTime::Now())
			, m_lastRunTime(DateTime::Now())
			, m_callback(callback)
			, m_state(state)
			, m_dueTime(m_startTime + dueTime)
			, m_period(period)
			, m_waitHandle(NULL)
		{

		}

		explicit TimerRunnable(TimerCallbackPtr callback, StatePtr state, TimeSpan dueTime,	TimeSpan period, HANDLE waitHandle)
			: m_startTime(DateTime::Now())
			, m_lastRunTime(DateTime::Now())
			, m_callback(callback)
			, m_state(state)
			, m_dueTime(m_startTime + dueTime)
			, m_period(period)
			, m_waitHandle(waitHandle)
		{

		}

		~TimerRunnable()
		{
			Close();
		}

		virtual DWORD Run()
		{
			m_lastRunTime = DateTime::Now();
			return EXIT_SUCCESS;
		}

		void Close()
		{
			if(m_waitHandle != NULL)
			{
				EARLGREY_ASSERT(::ResetEvent(m_waitHandle));
				m_waitHandle = NULL;
			}
		}

		BOOL IsExpired() const
		{
			DateTime now(DateTime::Now());
			return now > m_dueTime;
		}



	private:
		DateTime m_startTime;
		DateTime m_lastRunTime;

		HANDLE m_waitHandle;

		TimerCallbackPtr m_callback;
		StatePtr m_state;
		DateTime m_dueTime;
		TimeSpan m_period;
	};
}