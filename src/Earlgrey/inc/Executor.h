#pragma once

#include "Thread.h"
#include "Proactor.h"

namespace Earlgrey {

	class TaskCompletionHandler : public CompletionHandler
	{
	public:
		explicit TaskCompletionHandler(std::tr1::shared_ptr<IRunnable> task) : Task_(task) {};
		virtual ~TaskCompletionHandler() {};

		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes) {

			UNREFERENCED_PARAMETER(Result);
			EARLGREY_ASSERT(TransferredBytes == 0);
			Task_->Run();

		}

		virtual void HandleEventError(AsyncResult* Result, DWORD Error) {
			UNREFERENCED_PARAMETER(Result);
			UNREFERENCED_PARAMETER(Error);

		}

	private:
		std::tr1::shared_ptr<IRunnable> Task_;
	};



	//  Proactor Executor Service
	// TODO ; ICallable, IFuture 를 구현해보자!!

	class Executor : private Uncopyable
	{
		friend struct Loki::CreateUsingNew<Executor>;

	private:
		explicit Executor()
		{
		}

	public:
		void Execute(std::tr1::shared_ptr<IRunnable> task) {
			// TODO ; check TaskCompletionHandler life cycle 

			ProactorSingleton::Instance().PostEvent(new AsyncResult(new TaskCompletionHandler(task)));

		}

		void Shutdown() 
		{
		}

	private:

	};

	typedef
		Loki::SingletonHolder<Executor, Loki::CreateUsingNew, Loki::DefaultLifetime,  Loki::SingleThreaded, NoLock> 
		IocpExecutorSingleton
		;
}