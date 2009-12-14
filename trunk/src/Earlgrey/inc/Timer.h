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
		 // friend struct Loki::CreateUsingNew<Timer>;
		 typedef std::tr1::shared_ptr<TimerCallback> TimerCallbackPtr;
 
	public:
		/*
		std::tr1::shared_ptr<Timer> NewInstance(
			TimerCallbackPtr callback
			, void * state
			, TimeSpan dueTime
			, TimeSpan period
			);

		std::tr1::shared_ptr<Timer> NewInstance(TimerCallbackPtr callback);
		*/

		explicit Timer(
			TimerCallbackPtr callback
			, void * state
			, TimeSpan dueTime
			, TimeSpan period
			);
		explicit Timer(TimerCallbackPtr callback);

		void Change(TimeSpan dueTime, TimeSpan period);

		void Close();
		void Close(HANDLE waitHandle);

	private: // private methods

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