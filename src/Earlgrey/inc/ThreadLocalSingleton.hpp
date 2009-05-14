#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "ThreadLocal.hpp"
#include "Loki/Singleton.h"

namespace Earlgrey
{
	template <class T>
	struct ThreadLocalLifetime
	{
		static void ScheduleDestruction(T*, Loki::atexit_pfn_t pFun)
		{ std::set_terminate(pFun); }

		static void OnDeadReference()
		{ throw std::logic_error("Dead Reference Detected"); }
	};

	template <class Host, class MutexPolicy = LOKI_DEFAULT_MUTEX>
	class ThreadLocalThreaded
	{
	public:
		/// \struct Lock
		/// Dummy Lock class
		struct Lock
		{
			Lock() {}
			explicit Lock(const ThreadLocalThreaded&) {}
			explicit Lock(const ThreadLocalThreaded*) {}
		};

		typedef ThreadLocal<Host> VolatileType;

	
	};

	// ThreadingModel<T*,MutexPolicy>::VolatileType 

}
