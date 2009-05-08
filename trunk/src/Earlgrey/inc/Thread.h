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
		explicit Thread();

	public:	
		BOOL Create(IRunnable* runnable, LPCSTR threadName, DWORD stackSize = 0);

		//
		// Usage: SetThreadName (-1, "MainThread");
		//
		static void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName);

	private:
		static unsigned int __stdcall _ThreadProc(LPVOID p);
		DWORD Run();
		void ResetRunnableObject();

	private:
		HANDLE _thread;
		IRunnable* _runnable;
	};
}