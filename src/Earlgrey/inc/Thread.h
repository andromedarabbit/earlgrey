#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"
#include "ThreadId.h"
#include "ThreadLocal.hpp"

#undef max
#undef min

#include <limits>

namespace Earlgrey
{
	class ThreadRunnable;
	class Thread;

	typedef std::tr1::shared_ptr<Thread> ThreadPtr;

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
		// static const ThreadIdType INVALID_THREAD_ID = std::numeric_limits<ThreadIdType>::max();
		// static const ThreadIdType INVALID_THREAD_ID = IO

		enum {
			Running = 0,
			Suspended = CREATE_SUSPENDED
		};

		explicit Thread();
		~Thread();

		inline bool IsCreated() const 
		{ 
			return ThreadHandle_ != NULL; 
		}

		inline ThreadIdType NativeThreadId() const 
		{ 
			EARLGREY_ASSERT( NativeThreadId_ != INVALID_THREAD_ID );
			return NativeThreadId_; 
		}

		inline ThreadIdType ThreadId() const 
		{ 
			EARLGREY_ASSERT( ThreadId_ != INVALID_THREAD_ID );
			return ThreadId_; 
		}

	public:
		// Factory 
		static std::tr1::shared_ptr<Thread> CreateThread(std::tr1::shared_ptr<ThreadRunnable> runnable, LPCSTR threadName, ThreadIdType threadId, DWORD stackSize = 0);
		static std::tr1::shared_ptr<Thread> AttachThread(LPCSTR threadName, ThreadIdType threadId);
		static std::tr1::shared_ptr<Thread> CurrentThread();

	public:	
		void SetName(LPCSTR threadName);
		void SetProcessorAffinity(DWORD indexOfProcessor, DWORD countOfProcessor);
		void SetPriority(INT priority);

		void Join() {
			EARLGREY_ASSERT(ThreadHandle_ != NULL);
		
			WaitForSingleObject(ThreadHandle_, INFINITE);
		}

		void Stop();

	private:
		BOOL Create(
			std::tr1::shared_ptr<ThreadRunnable> runnable
			, class ThreadHolder* threadHolder
			, LPCSTR threadName
			, ThreadIdType threadId
			, ThreadIdType initFlag = Running
			, DWORD stackSize = 0
			);
		DWORD Run();

	private:
		HANDLE	ThreadHandle_;
		ThreadIdType NativeThreadId_;
		ThreadIdType ThreadId_;
		std::tr1::shared_ptr<ThreadRunnable> Runnable_;
		BOOL IsRunning_; // TODO atomic check ÇÊ¿ä
		HANDLE	CreatedLock_;

	private:
		static ThreadLocalValue<std::tr1::shared_ptr<Thread>> CurrentThread_;

	};

	
}