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
			std::auto_ptr<ThreadHolder> This = std::auto_ptr<ThreadHolder>( reinterpret_cast<ThreadHolder*>( p ) );
			EARLGREY_ASSERT(This->Thread_ != NULL);
			DWORD exitCode = This->Thread_->Run();
			return exitCode;
		}

	private:
		std::tr1::shared_ptr<Thread> Thread_;

	};

	ThreadLocalValue<std::tr1::shared_ptr<Thread>> Thread::CurrentThread_ = std::tr1::shared_ptr<Thread>();
	

	Thread::Thread() 
		: ThreadHandle_(NULL)
		, ThreadId_(INVALID_THREAD_ID)
		, IsRunning_(FALSE)
	{
		CreatedLock_ = CreateMutex(NULL, true, NULL);
		EARLGREY_VERIFY(CreatedLock_ != NULL);
	}

	Thread::~Thread()
	{
		if (!IsCreated())
			return;

		// WaitForSingleObject( _thread, WaitTimeForThread );
		CloseHandle(CreatedLock_);
		// SetEvent(_thread);
		CloseHandle( ThreadHandle_ );
	}

	void Thread::SetName(LPCSTR threadName)
	{
		EARLGREY_ASSERT( ThreadId_ != INVALID_THREAD_ID );
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = threadName;
		info.dwThreadID = ThreadId_;
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
		CurrentThread_ = std::tr1::shared_ptr<Thread>(this);

		EARLGREY_VERIFY( NULL != Runnable_ );
		EARLGREY_VERIFY( Runnable_->Init() );

		IsRunning_ = TRUE;

		ReleaseMutex(CreatedLock_);

		DWORD exitCode = Runnable_->Run();

		IsRunning_ = FALSE;
		Runnable_->Exit();

		// _endthread()는 thread handle(object)를 close하지만, _endthreadex()는 close하지 않는다. 따라서, CloseHandle()은 따로 호출해야 한다.
		_endthreadex( 0 );
		return exitCode;
	}

	BOOL Thread::Create(
				std::tr1::shared_ptr<IRunnable>		runnable, 
				ThreadHolder*						threadHolder, 
				LPCSTR								threadName, 
				unsigned int						initFlag, 
				DWORD								stackSize)
	{
		Runnable_ = runnable;

		// 오류 종류에 따라 -1이나 0을 반환한다.
		uintptr_t threadHandle = _beginthreadex( NULL, stackSize, ThreadHolder::_ThreadProc, threadHolder, initFlag, &ThreadId_ );		
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

		WaitForSingleObject(CreatedLock_, INFINITE);

		ThreadHandle_ = reinterpret_cast<HANDLE>( threadHandle );

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
		EARLGREY_VERIFY(NULL != thread);
		if (thread)
		{
			thread->Create( runnable,  new ThreadHolder(thread), threadName, Thread::Running, stackSize );
		}

		return std::tr1::shared_ptr<Thread>(thread);
	}


	class MainRunnable : public IRunnable {

		virtual BOOL Init() { return TRUE; };
		virtual DWORD Run() {
			return 0;
		}
		;
		virtual void Stop() {};
		virtual void Exit() {};

	};

	std::tr1::shared_ptr<Thread> Thread::AttachThread(LPCSTR threadName)
	{
		Thread* thread = new Thread();

		thread->ThreadId_ = GetCurrentThreadId();
		thread->SetName(threadName);
		thread->Runnable_ = std::tr1::shared_ptr<IRunnable>(new MainRunnable());

		CurrentThread_ = std::tr1::shared_ptr<Thread>(thread);


		return CurrentThread_;
	}


	std::tr1::shared_ptr<Thread> Thread::CurrentThread()
	{
		return CurrentThread_.Get();
	}

	void Thread::SetProcessorAffinity(DWORD indexOfProcessor, DWORD countOfProcessor)
	{
		SetProcessAffinityMask( ThreadHandle_, 1 << (indexOfProcessor % countOfProcessor) );
	}

	void Thread::SetPriority(INT priority)
	{
		EARLGREY_ASSERT(
			priority >= THREAD_PRIORITY_IDLE &&
			priority <= THREAD_PRIORITY_TIME_CRITICAL
			);

		EARLGREY_VERIFY( SetThreadPriority(ThreadHandle_, priority) );
	}

	
}