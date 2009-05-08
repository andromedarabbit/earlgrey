#pragma once
#include "Uncopyable.h"

namespace Earlgrey
{

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

	private:
		explicit Thread(); // 일단 static 클래스로 해 놓았다.

	public:	

		//
		// Usage: SetThreadName (-1, "MainThread");
		//
		static void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName);
		
	};


}