#pragma once
#include "Thread.h"
#include "Runnable.h"
#include "Proactor.h"
#include "taskqueue.h"

#include "xqueue.h"
// #include "LockfreeQueue.hpp"
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

	/*
	template<class T>
	class ICallable {

	T call();

	};

	template<class T>
	class IFuture {

	T get();

	};
	*/



	//  Proactor Executor Service
	// TODO ; ICallable, IFuture 를 구현해보자!!
	class Executor : public Algorithm::Lockfree::TaskQueue
		// : private Uncopyable
	{
		friend struct Loki::CreateUsingNew<Executor>;
		friend class ExecutorTaskRunnerInvoker; // DoTask
		friend class ThreadRunnable;

	private:
		explicit Executor()
		{
		}

	public:
		typedef std::tr1::shared_ptr<IRunnable> Task;
		typedef xqueue<Task>::Type Tasks;
		typedef std::queue<Task> Tasks;
		// typedef Algorithm::Lockfree::Queue<Task> Tasks;
		typedef std::tr1::array<Tasks, MAX_THREADS> ThreadTasks;

		void Execute(Task task);
		void Execute(Task task, ThreadIdType threadId);

		// void Shutdown(); cleanup, 일단은 필요없으니.

	private: // private methods
		void AddTask_(Task task, ThreadIdType threadId);

		void DoTasks();
		void DoTasks_();


	private: // private fields
		ThreadTasks m_threadTasks;
	};

	typedef
		Loki::SingletonHolder<Executor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		IocpExecutorSingleton
		;

	/*
	// IOCP Thread Loop 로 돌아자지 않고, ThreadTask Queue 를 비울때 사용. 이런거 자동으로 해주는 방법이 있었으면 좋겠다.
	// per thread event 를 kernel 에서 주기적으로 발생시키는 -.- runner 

	class ExecutorTaskRunnerInvoker 
	{
		// TODO ; heap 생성을 막자.
	public:
		~ExecutorTaskRunnerInvoker() 
		{
			IocpExecutorSingleton::Instance().DoTasks();
		}

	};
	*/
}