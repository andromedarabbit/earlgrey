#include "stdafx.h"
#include "Executor.h"

namespace Earlgrey
{
	TaskCompletionHandler::~TaskCompletionHandler()
	{

	}

	void TaskCompletionHandler::HandleEvent(AsyncResult* Result) {

		UNREFERENCED_PARAMETER(Result);
		EARLGREY_ASSERT(Result->GetBytesTransferred() == 0);
		Task_->Run();

	}

	Executor::Executor()
	{

	}

	// TODO ; check TaskCompletionHandler life cycle
	void Executor::Execute(Task task) 
	{ 
		ProactorSingleton::Instance().PostEvent(new AsyncResult(new TaskCompletionHandler(task)));
	}

	/*
	void Executor::Execute(Task task, ThreadIdType threadId) 
	{
	EARLGREY_ASSERT(IsValidIOThreadId(threadId));
	m_threadTasks[threadId].Enqueue(task);
	}

	void Executor::DoTasks()
	{
	const ThreadIdType tid = Thread::CurrentThread()->ThreadId();
	Tasks& threadTasks = m_threadTasks[tid];

	Task task;
	while(threadTasks.Dequeue(task))
	{
	task->Run();
	}
	} 
	*/

	void Executor::Execute(Task task, ThreadIdType threadId) 
	{
		InvokeMethod( &Executor::PushTask_, task, threadId );
	}

	void Executor::PushTask_(Task task, ThreadIdType threadId)
	{
		EARLGREY_ASSERT(IsValidIOThreadId(threadId));
		m_threadTasks[threadId].push(task);
	}

	void Executor::DoTasks()
	{
		InvokeMethod(&Executor::DoTasks_);
	}

	//! \todo 한번에 너무 많이 처리하면 안 되므로 최대 수행시간을 받아야 한다. 
	/// 고해상도 타이머가 필요해서 일단 대충 넘긴다.
	//! \todo 큐 대신 우선순위 큐를 골라 긴급요청부터 처리할까? 
	// 이러면 스레드 스케줄러처럼 starvation 문제를 해결해야 한다.
	void Executor::DoTasks_()
	{
		const ThreadIdType tid = Thread::CurrentThread()->ThreadId();
		Tasks& threadTasks = m_threadTasks[tid];

		while(threadTasks.empty() == false)
		{
			Task task = threadTasks.front();
			threadTasks.pop();
			task->Run();
		}
	} 

	/*
	void Executor::Shutdown() 
	{
	}
	*/
}