#include "stdafx.h"
#include "Thread.h"
#include "EarlgreyAssert.h"
#include <process.h>

namespace Earlgrey
{
	Thread::Thread() : _thread(NULL), _runnable(NULL) 
	{
	}

	void Thread::SetThreadName( DWORD dwThreadID, LPCSTR szThreadName)
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = szThreadName;
		info.dwThreadID = dwThreadID;
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

	void Thread::ResetRunnableObject()
	{
		delete _runnable;
		_runnable = NULL;
	}


	DWORD Thread::Run()
	{
		EARLGREY_VERIFY( _runnable );
		EARLGREY_VERIFY( _runnable->Init() );
		DWORD exitCode = _runnable->Run();
		_runnable->Exit();

		ResetRunnableObject();

		return exitCode;
	}

	BOOL Thread::Create(IRunnable* runnable, LPCSTR threadName, DWORD stackSize)
	{
		unsigned int threadId = 0;
		_runnable = runnable;

		_thread = reinterpret_cast<HANDLE>( _beginthreadex( NULL, stackSize, _ThreadProc, this, 0, &threadId ) );
		EARLGREY_VERIFY( _thread != (HANDLE)-1L );
		if (_thread == (HANDLE)-1L)
		{
			// thread creation error
			return FALSE;
		}

		SetThreadName( threadId, threadName );

		return TRUE;
	}
}