#include "stdafx.h"
#include "Thread.h"
#include "ThreadMessage.h"
#include "Runnable.h"
#include "shared_ptr_helper.h"

namespace Earlgrey { namespace Test 
{
	using std::tr1::shared_ptr;

	class ThreadMessageRunnable : public IRunnable
	{
	public:
		ThreadMessageRunnable()
			: IsStarted(FALSE), IsRunned(FALSE), IsStopped(FALSE)
		{

		}
		~ThreadMessageRunnable()
		{

		}
		BOOL Init() 
		{ 
			IsStarted = TRUE;
			return TRUE; 
		}
		DWORD Run() 
		{
			CurrentThread = Thread::CurrentThread();
			Sleep(1000);
			IsRunned = TRUE;
			return 0;
		}

		void Stop() 
		{
			IsStopped = TRUE;

		}
		void Exit() {}


	public:
		std::tr1::shared_ptr<Thread> CurrentThread;
		BOOL IsStarted;
		BOOL IsRunned;
		BOOL IsStopped;
	};

	class IncreaseCountThreadMessage : public IThreadMessage
	{
	public:

		virtual ~IncreaseCountThreadMessage() 
		{
			InterlockedIncrement(&DestructCount);
		}
		static long DestructCount;
		static int Count;

		virtual void OnReceive()
		{
			Count++;
		}
	};

	long IncreaseCountThreadMessage::DestructCount = 0;
	int IncreaseCountThreadMessage::Count = 0;

	TEST(ThreadMessageQueue, ThreadMessage) 
	{
		const int queueSize = 100;
		
		ThreadMessageQueue queue(Thread::CurrentThread());


		for (int i = 0; i < queueSize; i++) 
		{
			std::tr1::shared_ptr<IncreaseCountThreadMessage> message(new IncreaseCountThreadMessage() );


			queue.Enqueue(message);

		}


		queue.InvokeAll();

		EXPECT_EQ(queueSize, IncreaseCountThreadMessage::Count);
		//EXPECT_EQ(queueSize, IncreaseCountThreadMessage::DestructCount);
		


	}
}}