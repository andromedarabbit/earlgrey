#pragma once 
#include "Uncopyable.h"
#include "ScopedLock.h"

namespace Earlgrey
{
	class Win32Lock : private Uncopyable
	{
	public:
		inline explicit Win32Lock() {
			InitializeCriticalSection(&m_CriticalSection);
		}
		~Win32Lock() {}

		inline void Lock() {
			EnterCriticalSection(&m_CriticalSection);
		}
		inline void UnLock() {
			LeaveCriticalSection(&m_CriticalSection);
		}	
	private:
		CRITICAL_SECTION m_CriticalSection;
	};

	typedef TScopedLock<Win32Lock> ScopedLock;
}

