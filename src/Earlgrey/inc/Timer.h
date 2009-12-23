#pragma once
#include "TimerCallback.h"
#include "TimeSpan.h"
#include "TimerRunnable.h"
// #include "ThreadLocalSingleton.hpp"

namespace Earlgrey
{
	class TimeSpan;

	//1 \todo 스택 할당 방지하기 
	class Timer : private Uncopyable
	{
	public:
		explicit Timer(
			TimerCallback callback
			, StatePtr state
			, TimeSpan dueTime
			, TimeSpan period
			);
		explicit Timer(TimerCallback callback);

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

		TimerRunnable::IDType m_runnableID;
	};

	/*
	typedef 
		Loki::SingletonHolder<Timer, Loki::CreateUsingNew, ThreadLocalLifetime, ThreadLocalThreaded> 
		TimerSingleton;
		*/
}