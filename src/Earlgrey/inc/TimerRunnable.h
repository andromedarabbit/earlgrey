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
		typedef LONG IDType;
		static const IDType INVALID_ID = 0; 

		explicit TimerRunnable(TimerCallback callback, StatePtr state, TimeSpan dueTime, TimeSpan period)
			: m_id(GenerateID())
			, m_lastRunTime(DateTime::Now())
			, m_callback(callback)
			, m_state(state)
			, m_dueTime(DateTime::Now() + dueTime)
			, m_period(period)
			// , m_waitHandle(NULL)
			, m_expired(FALSE)
		{

		}

		~TimerRunnable()
		{
			// Close();
		}

		virtual DWORD Run();
		// void Close();

		inline IDType ID() const
		{
			return m_id;
		}

		inline BOOL IsExpired() const
		{
			return m_expired;
		}

	private:
		static IDType GenerateID();
		static IDType TIMER_COUNT; 

	private:
		IDType m_id;
		DateTime m_lastRunTime;

		// HANDLE m_waitHandle;

		TimerCallback m_callback;
		StatePtr m_state;
		DateTime m_dueTime;
		TimeSpan m_period;
		BOOL m_expired;
	};

	LONG TimerRunnable::TIMER_COUNT = TimerRunnable::INVALID_ID;



	class TimerDeregisterMessage : private Uncopyable
	{
	public:
		explicit TimerDeregisterMessage(TimerRunnable::IDType id)
			: m_id(id)
			, m_waitHandle(NULL)
		{

		}

		explicit TimerDeregisterMessage(TimerRunnable::IDType id, HANDLE waitHandle)
			: m_id(id)
			, m_waitHandle(waitHandle)
		{

		}

		inline TimerRunnable::IDType ID() const 
		{
			return m_id;
		}

		void Close()
		{
			if(m_waitHandle == NULL)
				return;
			EARLGREY_ASSERT(::ResetEvent(m_waitHandle));
		}

	private:
		TimerRunnable::IDType m_id;
		HANDLE m_waitHandle;
	};

}