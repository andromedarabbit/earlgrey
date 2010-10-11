#pragma once
#include "Uncopyable.h"
#include "txstring.h"

// #include <Loki/Singleton.h>
// #include <Loki/Threads.h> // for Loki::SingleThreaded
// #include "NoLock.h"

// #include "taskqueue.h"

namespace Earlgrey
{
	class Log 
		: private Uncopyable
		// : public Algorithm::Lockfree::TaskQueue
	{
	private:
		explicit Log();

	public: // class methods 
		//! \todo 임시 구현
		//! \ref http://recoverlee.tistory.com/9
		static _txstring ErrorMessage(DWORD errorCode, HMODULE source);
		static _txstring ErrorMessage(DWORD errorCode);

	};

}