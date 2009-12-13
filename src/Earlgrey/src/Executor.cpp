#include "stdafx.h"
#include "Executor.h"

namespace Earlgrey
{
	TaskCompletionHandler::~TaskCompletionHandler()
	{

	}

	void TaskCompletionHandler::HandleEvent(AsyncResult* Result, DWORD TransferredBytes) {

		UNREFERENCED_PARAMETER(Result);
		EARLGREY_ASSERT(TransferredBytes == 0);
		Task_->Run();

	}

	void TaskCompletionHandler::HandleEventError(AsyncResult* Result, DWORD Error) {
		UNREFERENCED_PARAMETER(Result);
		UNREFERENCED_PARAMETER(Error);

	}


	// TODO ; check TaskCompletionHandler life cycle
	void Executor::Execute(Task task) 
	{ 
		ProactorSingleton::Instance().PostEvent(new AsyncResult(new TaskCompletionHandler(task)));
	}



	void Executor::Execute(Task task, ThreadIdType threadId) 
	{
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

	/*
	void Executor::Execute(Task task, ThreadIdType threadId) 
	{
		InvokeMethod( &Executor::AddTask_, this, task, threadId );
	}

	void Executor::AddTask_(Task task, ThreadIdType threadId)
	{
		m_threadTasks[threadId].push(task);
	}

	void Executor::DoTasks()
	{
		InvokeMethod(&Executor::DoTasks_, this);
	}

	//! \todo 한번에 너무 많이 처리하면 안 되므로 최대 수행시간을 받아야 한다. 
	/// 고해상도 타이머가 필요해서 일단 대충 넘긴다.
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
	*/

	/*
	void Executor::Shutdown() 
	{
	}
	*/
}