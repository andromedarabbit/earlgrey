#include "stdafx.h"
#include "Thread.h"
#include "EarlgreyAssert.h"
#include <process.h>

namespace Earlgrey
{
	Thread::Thread() : _thread(NULL), _threadId(0)
	{
	}

	void Thread::SetName(LPCSTR threadName)
	{
		EARLGREY_ASSERT( _threadId != 0 );
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = threadName;
		info.dwThreadID = _threadId;
		info.dwFlags = 0;

		__try
		{
			RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(DWORD), (DWORD_PTR*)&info );
		}
		__except(EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}

	unsigned int __stdcall Thread::_ThreadProc(LPVOID p)
	{
		Thread* This = reinterpret_cast<Thread*>( p );
		DWORD exitCode = This->Run();
		delete This;
		return exitCode;
	}

	DWORD Thread::Run()
	{
		EARLGREY_VERIFY( _runnable );
		EARLGREY_VERIFY( _runnable->Init() );
		DWORD exitCode = _runnable->Run();
		_runnable->Exit();
		return exitCode;
	}

	BOOL Thread::Create(std::tr1::shared_ptr<IRunnable> runnable, LPCSTR threadName, unsigned int initFlag, DWORD stackSize)
	{
		_runnable = runnable;

		_thread = reinterpret_cast<HANDLE>( _beginthreadex( NULL, stackSize, _ThreadProc, this, initFlag, &_threadId ) );
		EARLGREY_VERIFY( _thread != (HANDLE)-1L );
		if (_thread == (HANDLE)-1L)
		{
			// thread creation error
			return FALSE;
		}

		if (threadName && strlen(threadName) > 0)
		{
			SetName( threadName );
		}

		return TRUE;
	}

	Thread* Thread::CreateRunningThread(std::tr1::shared_ptr<IRunnable> runnable, LPCSTR threadName, DWORD stackSize)
	{
		EARLGREY_ASSERT( runnable );
		Thread* thread = new Thread();
		EARLGREY_ASSERT( thread );
		if (thread)
			thread->Create( runnable, threadName, Thread::Running, stackSize );

		return thread;
	}

	void Thread::SetProcessorAffinity(DWORD indexOfProcessor, DWORD countOfProcessor)
	{
		SetProcessAffinityMask( _thread, 1 << (indexOfProcessor % countOfProcessor) );
	}

	void Thread::SetPriority(INT priority)
	{
		EARLGREY_ASSERT(priority >= THREAD_PRIORITY_IDLE &&
			priority <= THREAD_PRIORITY_TIME_CRITICAL);

		SetThreadPriority(_thread, priority);
	}
}