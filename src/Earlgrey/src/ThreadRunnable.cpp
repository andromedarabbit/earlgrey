#include "stdafx.h"
#include "ThreadRunnable.h"
#include "Thread.h"
#include "Executor.h"

namespace Earlgrey
{
	ThreadRunnable::ThreadRunnable() 		
	{
	}

	ThreadRunnable::~ThreadRunnable()
	{

	}

	BOOL ThreadRunnable::Init()
	{
		EARLGREY_ASSERT( IsValidIOThreadId(Thread::CurrentThread()->ThreadId()) );
		return TRUE;
	}

	DWORD ThreadRunnable::Run()
	{
		while(MeetsStopCondition() == FALSE)
		{
			ExecutorTaskRunnerInvoker Invoker;

			const DWORD errCode = DoTask();
			if(errCode != EXIT_SUCCESS)
				return errCode; 
		}

		return EXIT_SUCCESS;
	}


}