#pragma once
#include "Thread.h"
#include "taskqueue.h"
#include "CompletionHandler.h"
#include "NoLock.h"

namespace Earlgrey 
{
	class AsyncResult;
	class IRunnable;

	class TaskCompletionHandler : public CompletionHandler
	{
	public:
		explicit TaskCompletionHandler(std::tr1::shared_ptr<IRunnable> task);

		virtual ~TaskCompletionHandler();

		virtual void HandleEvent(AsyncResult* Result);

	private:
		std::tr1::shared_ptr<IRunnable> Task_;
	};





	//  Proactor Executor Service
	// TODO ; ICallable, IFuture 를 구현해보자!!
	class Executor : public Algorithm::Lockfree::TaskQueue<void()>
		// : private Uncopyable
	{
		friend struct Loki::CreateUsingNew<Executor>;
		friend class ExecutorTaskRunnerInvoker; // PushTask

	public:
		typedef std::tr1::shared_ptr<IRunnable> Task;
		//! \todo 나중에 x 계열 컨테이너로 교체한다.
		// typedef xqueue<Task>::Type Tasks;
		typedef std::queue<Task> Tasks;
		// typedef std::list<Task> Tasks;
		// typedef Algorithm::Lockfree::Queue<Task> Tasks;
		typedef std::tr1::array<Tasks, MAX_THREADS> ThreadTasks;

		void Execute(Task task);
		void Execute(Task task, ThreadIdType threadId);

		// void Shutdown(); cleanup, 일단은 필요없으니.

	private: // private methods
		explicit Executor();

		void PushTask_(Task task, ThreadIdType threadId);

		void DoTasks();
		void DoTasks_();


	private: // private fields
		ThreadTasks m_threadTasks;
	};

	typedef Loki::SingletonHolder<Executor> IocpExecutorSingleton;


	class ExecutorTaskRunnerInvoker 
	{
	public:
		ExecutorTaskRunnerInvoker();

		~ExecutorTaskRunnerInvoker() 
		{
			
		}

	private: 
		// heap 생성을 막기.
		void * operator new(size_t);
		void * operator new[](size_t);
		void operator delete(void *);
		void operator delete[](void *);
	};
}