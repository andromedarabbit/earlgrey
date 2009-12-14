#include "stdafx.h"
#include "Thread.h"
#include "ThreadRunnable.h"
#include "shared_ptr_helper.h"

namespace Earlgrey { namespace Test 
{
	using std::tr1::shared_ptr;

	class TestRunnable : public ThreadRunnable
	{
	public:
		TestRunnable()
			: IsStarted(FALSE), IsRunned(FALSE), IsStopped(FALSE)
		{
		}

		virtual ~TestRunnable()
		{
		}

	protected:
		virtual BOOL Init() 
		{ 
			IsStarted = TRUE;
			return TRUE; 
		}

		virtual BOOL MeetsStopCondition() const
		{
			return FALSE;
		}

		virtual DWORD DoTask()
		{
			CurrentThread = Thread::CurrentThread().get();
			Sleep(1000);
			IsRunned = TRUE;
			return EXIT_FAILURE;
		}

		virtual void Stop() 
		{
			IsStopped = TRUE;
		}

		virtual void Exit() {}
	

	public:
		Thread* CurrentThread;
		BOOL IsStarted;
		BOOL IsRunned;
		BOOL IsStopped;
	};

	TEST(Win32CurrentThread, Thread) 
	{
		std::tr1::shared_ptr<Thread> currentThread = Thread::CurrentThread();


		EXPECT_FALSE(NULL == currentThread);
	}

	TEST(ThreadTest, Thread)
	{

		TestRunnable* runner1 = new TestRunnable();
		TestRunnable* runner2 = new TestRunnable();

		ThreadPtr testThread1
			= Thread::CreateThread(shared_ptr<TestRunnable>(runner1), "TestThread", 0 );

		ThreadPtr testThread2
			= Thread::CreateThread(shared_ptr<TestRunnable>(runner2), "TestThread", 0 );


		EXPECT_TRUE( NULL !=  testThread1);
		EXPECT_TRUE(testThread1->IsCreated());

		EXPECT_TRUE( NULL !=  testThread2);
		EXPECT_TRUE(testThread2->IsCreated());

		testThread1->Join();
		testThread2->Join();

		EXPECT_FALSE(runner1->CurrentThread == runner2->CurrentThread);
		EXPECT_TRUE(runner1->CurrentThread == testThread1.get());
		EXPECT_TRUE(runner2->CurrentThread == testThread2.get());


	}

	TEST(MultipleThreadTest, Thread)
	{

		ThreadPtr testThread1
			= Thread::CreateThread(shared_ptr<TestRunnable>(new TestRunnable()), "TestThread1", 0 );


		EXPECT_TRUE( NULL !=  testThread1);
		EXPECT_TRUE(testThread1->IsCreated());


		ThreadPtr testThread2
			= Thread::CreateThread(shared_ptr<TestRunnable>(new TestRunnable()), "TestThread2", 0 );


		EXPECT_TRUE( NULL !=  testThread2);
		EXPECT_TRUE(testThread2->IsCreated());


		ThreadPtr testThread3
			= Thread::CreateThread(shared_ptr<TestRunnable>(new TestRunnable()), "TestThread3", 0 );


		EXPECT_TRUE( NULL !=  testThread3);
		EXPECT_TRUE(testThread3->IsCreated());


		testThread1->Join();
		testThread2->Join();
		testThread3->Join();
	}

}
}
