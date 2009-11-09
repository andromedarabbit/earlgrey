#pragma once 
#include "Uncopyable.h"
#include "ScopedLock.h"

namespace Earlgrey
{
	class Lock : private Uncopyable
	{
	public:
		inline explicit Lock() {
			InitializeCriticalSection(&SpinkLock_);
		}
		~Lock() {}

		inline void Lock() {
			EnterCriticalSection(&SpinkLock_);
		}
		inline void UnLock() {
			LeaveCriticalSection(&SpinkLock_);
		}	
	private:
		CRITICAL_SECTION SpinkLock_;
	};
}

typedef TScopedLock<Lock> ScopedLock;