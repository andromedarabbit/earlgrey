#pragma once
#include "Thread.h"
#include "Runnable.h"
#include "taskqueue.h"
#include <queue>
#include <array>

namespace Earlgrey 
{
	class AsyncResult;

	class TaskCompletionHandler : public CompletionHandler
	{
	public:
		explicit TaskCompletionHandler(std::tr1::shared_ptr<IRunnable> task) 
			: Task_(task) 
		{

		}

		virtual ~TaskCompletionHandler();

		virtual void HandleEvent(AsyncResult* Result);

	private:
		std::tr1::shared_ptr<IRunnable> Task_;
	};





	//  Proactor Executor Service
	// TODO ; ICallable, IFuture �� �����غ���!!
	class Executor : public Algorithm::Lockfree::TaskQueue
		// : private Uncopyable
	{
		friend struct Loki::CreateUsingNew<Executor>;
		friend class ExecutorTaskRunnerInvoker; // PushTask

	public:
		typedef std::tr1::shared_ptr<IRunnable> Task;
		//! \todo ���߿� x �迭 �����̳ʷ� ��ü�Ѵ�.
		// typedef xqueue<Task>::Type Tasks;
		typedef std::queue<Task> Tasks;
		// typedef std::list<Task> Tasks;
		// typedef Algorithm::Lockfree::Queue<Task> Tasks;
		typedef std::tr1::array<Tasks, MAX_THREADS> ThreadTasks;

		void Execute(Task task);
		void Execute(Task task, ThreadIdType threadId);

		// void Shutdown(); cleanup, �ϴ��� �ʿ������.

	private: // private methods
		explicit Executor();

		void PushTask_(Task task, ThreadIdType threadId);

		void DoTasks();
		void DoTasks_();


	private: // private fields
		ThreadTasks m_threadTasks;
	};

	typedef
		Loki::SingletonHolder<Executor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		IocpExecutorSingleton
		;


	class ExecutorTaskRunnerInvoker 
	{
	public:
		ExecutorTaskRunnerInvoker()
		{
			IocpExecutorSingleton::Instance().DoTasks();
		}

		~ExecutorTaskRunnerInvoker() 
		{
			
		}

	private: 
		// heap ������ ����.
		void * operator new(size_t);
		void * operator new[](size_t);
		void operator delete(void *);
		void operator delete[](void *);
	};
}