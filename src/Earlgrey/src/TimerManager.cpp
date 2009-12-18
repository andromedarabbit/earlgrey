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
		EARLGREY_ASSERT(IsValidIOThreadId(threadId));
		m_threadTasks[threadId].push_back(task);
	}

	void TimerManager::DoTasks()
	{
		InvokeMethod(&TimerManager::DoTasks_, this);
	}

	//! \todo �ѹ��� �ʹ� ���� ó���ϸ� �� �ǹǷ� �ִ� ����ð��� �޾ƾ� �Ѵ�. 
	/// ���ػ� Ÿ�̸Ӱ� �ʿ��ؼ� �ϴ� ���� �ѱ��.
	//! \todo ť ��� �켱���� ť�� ��� ��޿�û���� ó���ұ�? 
	// �̷��� ������ �����ٷ�ó�� starvation ������ �ذ��ؾ� �Ѵ�.
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

			EARLGREY_ASSERT(task->Run());
		}

		if(hasExpiredTask)
			CleanExpiredTasks_(tid);
	} 

	void TimerManager::Deregister(Task task, ThreadIdType threadId) 
	{
		InvokeMethod( &TimerManager::PopTask_, this, task, threadId );
	}

	void TimerManager::PopTask_(Task task, ThreadIdType threadId)
	{
		EARLGREY_ASSERT(IsValidIOThreadId(threadId));
		m_threadTasks[threadId].remove(task);
	}

	void TimerManager::CleanExpiredTasks_(ThreadIdType tid)
	{
		using namespace std;

		struct Anonymous {
			static BOOL IsExpired ( Task task ) {
				return task->IsExpired();
			}
		};

		EARLGREY_ASSERT(IsValidIOThreadId(tid));
		Tasks& tasks = m_threadTasks[tid];
		remove_if(tasks.begin(), tasks.end(), Anonymous::IsExpired);
	}
}