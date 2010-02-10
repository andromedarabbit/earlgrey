#pragma once
#include "ScopedLock.h"

namespace Earlgrey {
	class Uncopyable;

	class Mutex 
		: private Uncopyable
	{
	public:
		/*
		explicit Mutex() 
			: MutexHandle_( CreateMutex(NULL, FALSE, NULL) ) 
		{
			EARLGREY_VERIFY(MutexHandle_ != NULL);
		}
		*/

		explicit Mutex(BOOL IsLocked = FALSE) 
			: MutexHandle_(CreateMutex(NULL, IsLocked, NULL)) 
		{
			EARLGREY_VERIFY(MutexHandle_ != NULL);
		}

		~Mutex() {
			EARLGREY_VERIFY(CloseHandle(MutexHandle_));
		}

		inline void Lock() {
			EARLGREY_VERIFY(WaitForSingleObject(MutexHandle_, 0) == WAIT_OBJECT_0);
		}

		inline void Unlock() {
			EARLGREY_VERIFY(ReleaseMutex(MutexHandle_));
		}
	private:
		HANDLE MutexHandle_;
	};

	typedef TScopedLock<Mutex> ScopedMutex;

};