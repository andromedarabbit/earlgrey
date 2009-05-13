#pragma once
#include "Uncopyable.h"

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
		explicit Thread();

	public:
		static Thread* CreateRunningThread(std::tr1::shared_ptr<IRunnable> runnable, LPCSTR threadName, DWORD stackSize = 0);

	public:	
		BOOL Create(std::tr1::shared_ptr<IRunnable> runnable, LPCSTR threadName, unsigned int initFlag = Running, DWORD stackSize = 0);
		void SetName(LPCSTR threadName);
		void SetProcessorAffinity(DWORD indexOfProcessor, DWORD countOfProcessor);

		//! \todo 요 함수는 나중에 적당한 곳에 옮겨야함...
		static DWORD GetProcessorCount()
		{	
			SYSTEM_INFO sysinfo;
			GetSystemInfo(&sysinfo);
			return sysinfo.dwNumberOfProcessors;	
		}

	private:
		static unsigned int __stdcall _ThreadProc(LPVOID p);
		DWORD Run();

	private:
		HANDLE _thread;
		unsigned int _threadId;
		std::tr1::shared_ptr<IRunnable> _runnable;
	};
}