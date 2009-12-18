#pragma once
#include "TimerCallback.h"
#include "TimeSpan.h"
// #include "ThreadLocalSingleton.hpp"

namespace Earlgrey
{
	class TimeSpan;

	//1 \todo 스택 할당 방지하기 
	class Timer : private Uncopyable
	{
	public:
		explicit Timer(
			TimerCallbackPtr callback
			, StatePtr state
			, TimeSpan dueTime
			, TimeSpan period
			);
		explicit Timer(TimerCallbackPtr callback);

		void Change(TimeSpan dueTime, TimeSpan period);

		void Close();
		void Close(HANDLE waitHandle);

	private: // private fields
		TimerCallbackPtr m_callback;
		StatePtr m_state;
		TimeSpan m_dueTime;
		TimeSpan m_period;
	};

	/*
	typedef 
		Loki::SingletonHolder<Timer, Loki::CreateUsingNew, ThreadLocalLifetime, ThreadLocalThreaded> 
		TimerSingleton;
		*/
}