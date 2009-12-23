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

		EARLGREY_ASSERT(IsValidIOThreadId(threadId));

		IsExpired isExpired(request->ID());
		Tasks& tasks = m_threadTasks[threadId];
		tasks.remove_if( isExpired );
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