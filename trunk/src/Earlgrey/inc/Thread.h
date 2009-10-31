#pragma once
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	class IRunnable
	{
	public:
		virtual BOOL Init() = 0;
		virtual DWORD Run() = 0;
		virtual void Stop() = 0;
		virtual void Exit() = 0;
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
		enum {
			Running = 0,
			Suspended = CREATE_SUSPENDED
		};

		enum { WaitTimeForThread = 10000 };

		Thread();
		~Thread();

		bool IsCreated() const { return _thread != NULL; }

	public:
		// Factory 
		static std::tr1::shared_ptr<Thread> CreateThread(std::tr1::shared_ptr<IRunnable> runnable, LPCSTR threadName, DWORD stackSize = 0);

	public:	
		void SetName(LPCSTR threadName);
		void SetProcessorAffinity(DWORD indexOfProcessor, DWORD countOfProcessor);
		void SetPriority(INT priority);

		//HANDLE GetWindowHandle() const { return _thread; }
		void Join()
		{
			EARLGREY_ASSERT(_thread != NULL);
		
			WaitForSingleObject(_thread, INFINITE);
		}

	private:
		BOOL Create(std::tr1::shared_ptr<IRunnable> runnable, class ThreadHolder* threadHolder, LPCSTR threadName, unsigned int initFlag = Running, DWORD stackSize = 0);
		DWORD Run();

	private:
		HANDLE _thread;
		unsigned int _threadId;
		std::tr1::shared_ptr<IRunnable> _runnable;
		BOOL IsRunning_; // TODO atomic check ÇÊ¿ä
		HANDLE	CreatedLock_;
	};
}