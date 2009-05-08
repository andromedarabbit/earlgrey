#include "stdafx.h"
#include "Thread.h"

namespace Earlgrey
{
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
}