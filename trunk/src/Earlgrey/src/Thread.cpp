#include "stdafx.h"
#include "Thread.h"
#include "EarlgreyAssert.h"

#include <process.h>
#include <errno.h>


namespace Earlgrey
{
	class ThreadHolder
	{
		friend class Thread;
	public:
		explicit ThreadHolder(Thread* thread)
			: Thread_(thread)
		{
			EARLGREY_ASSERT(Thread_ != NULL);
		}

	private:
		static unsigned int __stdcall _ThreadProc(LPVOID p)
		{
			// new created thread context
			ThreadHolder* This = reinterpret_cast<ThreadHolder*>( p );
			EARLGREY_ASSERT(This->Thread_ != NULL);
			DWORD exitCode = This->Thread_->Run();
			delete This;
			return exitCode;
		}

	private:
		std::tr1::shared_ptr<Thread> Thread_;

	};

	Thread::Thread() : _thread(NULL), _threadId(0), IsRunning_(FALSE)
	{
		CreatedLock_ = CreateMutex(NULL, true, NULL);
		EARLGREY_ASSERT(CreatedLock_ != NULL);
	}

	Thread::~Thread()
	{
		if (!IsCreated())
			return;

		// WaitForSingleObject( _thread, WaitTimeForThread );
		CloseHandle(CreatedLock_);
		// SetEvent(_thread);
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

	DWORD Thread::Run()
	{		
		EARLGREY_VERIFY( _runnable );
		EARLGREY_VERIFY( _runnable->Init() );

		IsRunning_ = TRUE;

		ReleaseMutex(CreatedLock_);

		DWORD exitCode = _runnable->Run();

		IsRunning_ = FALSE;
		_runnable->Exit();

		// _endthread()�� thread handle(object)�� close������, _endthreadex()�� close���� �ʴ´�. ����, CloseHandle()�� ���� ȣ���ؾ� �Ѵ�.
		_endthreadex( 0 );
		return exitCode;
	}

	BOOL Thread::Create(std::tr1::shared_ptr<IRunnable> runnable, ThreadHolder* threadHolder, LPCSTR threadName, unsigned int initFlag, DWORD stackSize)
	{
		_runnable = runnable;

		// ���� ������ ���� -1�̳� 0�� ��ȯ�Ѵ�.
		uintptr_t threadHandle = _beginthreadex( NULL, stackSize, ThreadHolder::_ThreadProc, threadHolder, initFlag, &_threadId );		
		EARLGREY_ASSERT(threadHandle != -1 && threadHandle != 0);
		
		//! \todo ���� ó���ϰų� ���� EARLGREY_ASSERT�� EARLGREY_VERIFY�� �ٲٱ�
		//! \remark _beginthread�� _beginthreadex�� ������ ��� �����ϴ� ���� �ٸ���. _beginthread returns 1L, _beginthreadex returns 0
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

		WaitForSingleObject(CreatedLock_, INFINITE);

		_thread = reinterpret_cast<HANDLE>( threadHandle );

		if (threadName && strlen(threadName) > 0)
		{
			SetName( threadName );
		}

		return TRUE;
	}

	std::tr1::shared_ptr<Thread> Thread::CreateThread(std::tr1::shared_ptr<IRunnable> runnable, LPCSTR threadName, DWORD stackSize)
	{
		EARLGREY_ASSERT( runnable );
		Thread* thread = new Thread();
		EARLGREY_ASSERT(thread);
		if (thread)
		{
			thread->Create( runnable,  new ThreadHolder(thread), threadName, Thread::Running, stackSize );
		}

		return std::tr1::shared_ptr<Thread>(thread);
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