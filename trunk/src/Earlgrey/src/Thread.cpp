#include "stdafx.h"
#include "Thread.h"
#include "EarlgreyAssert.h"

#include <process.h>
#include <errno.h>


namespace Earlgrey
{
	Thread::Thread() : _thread(NULL), _threadId(0)
	{
	}

	Thread::~Thread()
	{
		if (!IsCreated())
			return;

		WaitForSingleObject( _thread, WaitTimeForThread );
		CloseHandle( _thread );
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

		// _endthread()는 thread handle(object)를 close하지만, _endthreadex()는 close하지 않는다. 따라서, CloseHandle()은 따로 호출해야 한다.
		_endthreadex( 0 );
		return exitCode;
	}

	BOOL Thread::Create(std::tr1::shared_ptr<IRunnable> runnable, LPCSTR threadName, unsigned int initFlag, DWORD stackSize)
	{
		_runnable = runnable;

		// 오류 종류에 따라 -1이나 0을 반환한다.
		uintptr_t threadHandle = _beginthreadex( NULL, stackSize, _ThreadProc, this, initFlag, &_threadId );		
		EARLGREY_ASSERT(threadHandle != -1 && threadHandle != 0);
		
		//! \todo 오류 처리하거나 위의 EARLGREY_ASSERT를 EARLGREY_VERIFY로 바꾸기
		//! \remark _beginthread와 _beginthreadex는 실패할 경우 리턴하는 값이 다르다. _beginthread returns 1L, _beginthreadex returns 0
		if(threadHandle == 0)
		{			
			if(errno == EAGAIN) // if there are too many threads
			{
			}
			
			if(errno == EINVAL) // if the argument is invalid or the stack size is incorrect
			{
			}

			if(errno ==  EACCES) // in the case of insufficient resources (such as memory). 
			{
			}

			return FALSE;
		}

		if(threadHandle == 0) // in which case errno and _doserrno are set
		{
			return FALSE;
		}

		_thread = reinterpret_cast<HANDLE>( threadHandle );

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
		EARLGREY_ASSERT(
			priority >= THREAD_PRIORITY_IDLE &&
			priority <= THREAD_PRIORITY_TIME_CRITICAL
			);

		EARLGREY_VERIFY( SetThreadPriority(_thread, priority) );
	}

	
}