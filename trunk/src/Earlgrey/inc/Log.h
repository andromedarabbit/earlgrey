#pragma once
#include "Uncopyable.h"
#include "txstring.h"

#include <Loki/Singleton.h>
#include <Loki/Threads.h> // for Loki::SingleThreaded
#include "NoLock.h"

#include "taskqueue.h"

namespace Earlgrey
{
	class Log 
		: public Algorithm::Lockfree::TaskQueue
	{
	private:
		friend struct Loki::CreateStatic<Log>;
		explicit Log();

	public: // instance methods


	public: // class methods 
		//! \todo 임시 구현
		//! \ref http://recoverlee.tistory.com/9
		static _txstring ErrorMessage(DWORD errorCode, HMODULE source);
		static _txstring ErrorMessage(DWORD errorCode);
	};

	typedef 
		Loki::SingletonHolder<Log, Loki::CreateStatic, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		LogSingleton;
}