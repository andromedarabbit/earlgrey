#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "ThreadLocal.hpp"

namespace Earlgrey
{
	class IRunnable
	{
	public:
		virtual BOOL Init() = 0;
		virtual DWORD Run() = 0;
		//! \todo Stop은 언제 쓰나요? 실제로 쓰는 곳이 없네요.
		virtual void Stop() = 0;
		virtual void Exit() = 0;
	};

	class RunnableBuilder {
	public:
		static std::tr1::shared_ptr<IRunnable> NewRunnable(IRunnable* runnable) {
			return std::tr1::shared_ptr<IRunnable> (runnable);
		}
	};

	class Thread : private Uncopyable
	{
		friend class ThreadHolder;
	private:
		// #define MS_VC_EXCEPTION 0x406D1388
		static const int MS_VC_EXCEPTION = 0x406D1388;

		typedef struct tagTHREADNAME_INFO
		{
			DWORD dwType; // Must be 0x1000.
			LPCSTR szName; // Pointer to name (in user addr space).
			DWORD dwThreadID; // Thread ID (-1=caller thread).
			DWORD dwFlags; // Reserved for future use, must be zero.
		} THREADNAME_INFO;

	public:
		static const unsigned int INVALID_THREAD_ID = 0;

		enum {
			Running = 0,
			Suspended = CREATE_SUSPENDED
		};

		enum { WaitTimeForThread = 10000 };

		explicit Thread();
		~Thread();

		inline bool IsCreated() const 
		{ 
			return ThreadHandle_ != NULL; 
		}

		inline unsigned int Id() const 
		{ 
			EARLGREY_ASSERT( ThreadId_ != INVALID_THREAD_ID );
			return ThreadId_; 
		}

	public:
		// Factory 
		static std::tr1::shared_ptr<Thread> CreateThread(std::tr1::shared_ptr<IRunnable> runnable, LPCSTR threadName, DWORD stackSize = 0);
		static std::tr1::shared_ptr<Thread> AttachThread(LPCSTR threadName);
		static std::tr1::shared_ptr<Thread> CurrentThread();

	public:	
		void SetName(LPCSTR threadName);
		void SetProcessorAffinity(DWORD indexOfProcessor, DWORD countOfProcessor);
		void SetPriority(INT priority);

		void Join() {
			EARLGREY_ASSERT(ThreadHandle_ != NULL);
		
			WaitForSingleObject(ThreadHandle_, INFINITE);
		}

		void Stop()  {
			Runnable_->Stop();
		}

	private:
		BOOL Create(std::tr1::shared_ptr<IRunnable> runnable, class ThreadHolder* threadHolder, LPCSTR threadName, unsigned int initFlag = Running, DWORD stackSize = 0);
		DWORD Run();

	private:
		HANDLE	ThreadHandle_;
		unsigned int ThreadId_;
		std::tr1::shared_ptr<IRunnable> Runnable_;
		BOOL IsRunning_; // TODO atomic check 필요
		HANDLE	CreatedLock_;

	private:
		static ThreadLocalValue<std::tr1::shared_ptr<Thread>> CurrentThread_;

	};
}