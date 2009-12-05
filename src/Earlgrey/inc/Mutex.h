#pragma once
#include "ScopedLock.h"

namespace Earlgrey {
	class Uncopyable;

	class Mutex 
		: private Uncopyable
	{
	public:
		explicit Mutex() : MutexHandle_( CreateMutex(NULL, FALSE, NULL) ) {}

		explicit Mutex(BOOL IsLocked) : MutexHandle_(CreateMutex(NULL, IsLocked, NULL)) {}

		~Mutex() {
			CloseHandle(MutexHandle_);
		}

		inline void Lock() {
			WaitForSingleObject(MutexHandle_, 0);
		}

		inline void Unlock() {
			ReleaseMutex(MutexHandle_);
		}
	private:
		HANDLE MutexHandle_;
	};

	typedef TScopedLock<Mutex> ScopedMutex;

};