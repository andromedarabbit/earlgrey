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
		InvokeMethod( &Executor::PushTask_, this, task, threadId );
	}

	void Executor::PushTask_(Task task, ThreadIdType threadId)
	{
		EARLGREY_ASSERT(IsValidIOThreadId(threadId));
		m_threadTasks[threadId].push(task);
	}

	void Executor::DoTasks()
	{
		InvokeMethod(&Executor::DoTasks_, this);
	}

	//! \todo �ѹ��� �ʹ� ���� ó���ϸ� �� �ǹǷ� �ִ� ����ð��� �޾ƾ� �Ѵ�. 
	/// ���ػ� Ÿ�̸Ӱ� �ʿ��ؼ� �ϴ� ���� �ѱ��.
	//! \todo ť ��� �켱���� ť�� ��� ��޿�û���� ó���ұ�? 
	// �̷��� ������ �����ٷ�ó�� starvation ������ �ذ��ؾ� �Ѵ�.
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