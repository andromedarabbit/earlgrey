#include "stdafx.h"
#include "TimerManager.h"

#include <algorithm>

namespace Earlgrey
{
	TimerManager::TimerManager()
	{

	}

	void TimerManager::Register(Task task, ThreadIdType threadId) 
	{
		InvokeMethod( &TimerManager::PushTask_, this, task, threadId );
	}

	void TimerManager::PushTask_(Task task, ThreadIdType threadId)
	{
		EARLGREY_ASSERT(IsValidThreadId(threadId));
		m_threadTasks[threadId].push_back(task);
	}

	void TimerManager::DoTasks()
	{
		InvokeMethod(&TimerManager::DoTasks_, this);
	}

	//! \todo 한번에 너무 많이 처리하면 안 되므로 최대 수행시간을 받아야 한다. 
	/// 고해상도 타이머가 필요해서 일단 대충 넘긴다.
	//! \todo 큐 대신 우선순위 큐를 골라 긴급요청부터 처리할까? 
	// 이러면 스레드 스케줄러처럼 starvation 문제를 해결해야 한다.
	void TimerManager::DoTasks_()
	{
		const ThreadIdType tid = Thread::CurrentThread()->ThreadId();
		Tasks& threadTasks = m_threadTasks[tid];

		/*
		while(threadTasks.empty() == false)
		{
			Task task = threadTasks.front();
			threadTasks.pop_front();
			task->Run();
		}
		*/
		BOOL hasExpiredTask = FALSE;

		Tasks::iterator it = threadTasks.begin();
		for( ; it != threadTasks.end(); it++)
		{
			Task task(*it);
			if(task->IsExpired())
			{
				hasExpiredTask = TRUE;
				continue;
			}

#ifdef _UNICODE
			DWORD exitCode = task->Run();
			EARLGREY_ASSERT(exitCode == EXIT_SUCCESS);
#else
			task->Run();
#endif
		}

		if(hasExpiredTask)
			CleanExpiredTasks_(tid);
	} 

	void TimerManager::Deregister(DeregisterRequest request, ThreadIdType threadId) 
	{
		InvokeMethod( &TimerManager::PopTask_, this, request, threadId );
	}

	void TimerManager::PopTask_(DeregisterRequest request, ThreadIdType threadId)
	{
		using namespace std;

		class IsExpired
			: public not_equal_to<Task>
		{
		public:
			explicit IsExpired(TimerRunnable::IDType id)
				: m_id(id)
			{
				EARLGREY_ASSERT(m_id != TimerRunnable::INVALID_ID);
			}

			bool operator()(Task task) const
			{  
				return task->ID() == m_id; 
			}

		private:
			TimerRunnable::IDType m_id;
		};

		EARLGREY_ASSERT(IsValidThreadId(threadId));

		IsExpired isExpired(request->ID());
		Tasks& tasks = m_threadTasks[threadId];
		tasks.remove_if( isExpired );

		request->Close();
	}

	void TimerManager::CleanExpiredTasks_(ThreadIdType tid)
	{
		using namespace std;

		struct Anonymous {
			static BOOL IsExpired ( Task task ) {
				return task->IsExpired();
			}
		};

		EARLGREY_ASSERT(IsValidThreadId(tid));
		Tasks& tasks = m_threadTasks[tid];
		remove_if(tasks.begin(), tasks.end(), Anonymous::IsExpired);
	}
}