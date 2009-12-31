#include "stdafx.h"
#include "ThreadRunnable.h"
#include "Thread.h"
#include "Executor.h"
#include "TimerManager.h"

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
		EARLGREY_ASSERT( IsValidThreadId(Thread::CurrentThread()->ThreadId()) );
		return TRUE;
	}

	DWORD ThreadRunnable::Run()
	{
		while(MeetsStopCondition() == FALSE)
		{
			ExecutorTaskRunnerInvoker Invoker; 
			//IocpExecutorSingleton::Instance().DoTasks();
			TimerManagerSingleton::Instance().DoTasks();

			const DWORD errCode = DoTask();
			if(errCode != EXIT_SUCCESS)
				return errCode; 
		}

		return EXIT_SUCCESS;
	}


}