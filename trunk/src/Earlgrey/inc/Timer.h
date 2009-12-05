#pragma once
#include "TimerCallback.h"
#include "TimeSpan.h"
// #include "ThreadLocalSingleton.hpp"

namespace Earlgrey
{
	class TimeSpan;

	class Timer : private Uncopyable
	{
		 // friend struct Loki::CreateUsingNew<Timer>;
		 typedef std::tr1::shared_ptr<TimerCallback> TimerCallbackPtr;

	public:
		explicit Timer(TimerCallbackPtr callback, void * state, TimeSpan dueTime, TimeSpan period);
		explicit Timer(TimerCallbackPtr callback);

		void Change(TimeSpan dueTime, TimeSpan period);

		void Close();


	private: // private fields
		TimerCallbackPtr m_callback;
		void * m_state;
		TimeSpan m_dueTime;
		TimeSpan m_period;
	};

	/*
	typedef 
		Loki::SingletonHolder<Timer, Loki::CreateUsingNew, ThreadLocalLifetime, ThreadLocalThreaded> 
		TimerSingleton;
		*/
}