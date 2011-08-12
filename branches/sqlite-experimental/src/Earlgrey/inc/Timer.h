#pragma once
#include "TimerCallback.h"
#include "TimeSpan.h"
#include "TimerRunnable.h"
#include "Thread.h"

namespace Earlgrey
{
	class TimeSpan;

	//! \todo 스택 할당 방지하기 
	class Timer : private Uncopyable
	{
	public:
		explicit Timer(
			TimerCallback callback
			, StatePtr state
			, TimeSpan dueTime
			, TimeSpan period
			// , ThreadIdType tid = INVALID_THREAD_ID
			, ThreadIdType tid = Thread::CurrentThread()->ThreadId()
			);
		// explicit Timer(TimerCallback callback, ThreadIdType tid = INVALID_THREAD_ID);
		explicit Timer(TimerCallback callback, ThreadIdType tid);

		void Change(TimeSpan dueTime, TimeSpan period);

		void Close();
		void Close(HANDLE waitHandle);

	private:
		void RegisterTimer();

	private: // private fields
		TimerCallback m_callback;
		StatePtr m_state;
		TimeSpan m_dueTime;
		TimeSpan m_period;
		ThreadIdType m_tid;

		TimerRunnable::IDType m_runnableID;
	};
}