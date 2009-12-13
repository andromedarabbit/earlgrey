#pragma once
#include "Thread.h"
#include "Runnable.h"
#include "Proactor.h"
// #include "taskqueue.h"

// #include "xqueue.h"
#include "LockfreeQueue.hpp"
#include <array>

namespace Earlgrey 
{
	class TaskCompletionHandler : public CompletionHandler
	{
	public:
		explicit TaskCompletionHandler(std::tr1::shared_ptr<IRunnable> task) 
			: Task_(task) 
		{

		}

		virtual ~TaskCompletionHandler();

		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes);

		virtual void HandleEventError(AsyncResult* Result, DWORD Error);

	private:
		std::tr1::shared_ptr<IRunnable> Task_;
	};



	//  Proactor Executor Service
	// TODO ; ICallable, IFuture 를 구현해보자!!
	class Executor // : public Algorithm::Lockfree::TaskQueue
		// : private Uncopyable
	{
		friend struct Loki::CreateUsingNew<Executor>;
		friend class ThreadRunnable; // DoTask

	private:
		explicit Executor()
		{
		}

	public:
		typedef std::tr1::shared_ptr<IRunnable> Task;
		// typedef xqueue<Task>::Type Tasks;
		typedef Algorithm::Lockfree::Queue<Task> Tasks;
		typedef std::tr1::array<Tasks, MAX_THREADS> ThreadTasks;

		void Execute(Task task);
		void Execute(Task task, ThreadIdType threadId);

		// void Shutdown();

	private: // private methods
		// void AddTask_(Task task, ThreadIdType threadId);
		
		void DoTasks();
		// void DoTasks_();


	private: // private fields
		ThreadTasks m_threadTasks;
	};

	typedef
		Loki::SingletonHolder<Executor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		IocpExecutorSingleton
		;
}