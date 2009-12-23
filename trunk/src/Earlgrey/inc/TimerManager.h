#pragma once
#include "Thread.h"
#include "TimerRunnable.h"
#include "taskqueue.h"
#include "NoLock.h"

#include <Loki/Singleton.h>
#include <array>

namespace Earlgrey
{
	class TimerManager : public Algorithm::Lockfree::TaskQueue
		// : private Uncopyable
	{
		friend struct Loki::CreateUsingNew<TimerManager>;
		friend class Timer; // PopTask

	public:
		typedef std::tr1::shared_ptr<TimerRunnable> Task;
		//! \todo 나중에 x 계열 컨테이너로 교체한다.
		// typedef xlist<Task>::Type Tasks;
		typedef std::list<Task> Tasks;
		typedef std::tr1::shared_ptr<TimerDeregisterMessage> DeregisterRequest;
		// typedef Algorithm::Lockfree::Queue<Task> Tasks;
		typedef std::tr1::array<Tasks, MAX_THREADS> ThreadTasks;

	private: // private methods
		explicit TimerManager();
		
		void Register(Task task, ThreadIdType threadId);
		void PushTask_(Task task, ThreadIdType threadId);

		void Deregister(DeregisterRequest request, ThreadIdType threadId);
		void PopTask_(DeregisterRequest request, ThreadIdType threadId);

		void DoTasks();
		void DoTasks_();

		void CleanExpiredTasks_(ThreadIdType tid);

	private: // private fields
		ThreadTasks m_threadTasks;
	};

	typedef
		Loki::SingletonHolder<TimerManager, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		TimerManagerSingleton
		;

}