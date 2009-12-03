#include "stdafx.h"
#include "Thread.h"
#include "shared_ptr_helper.h"

namespace Earlgrey { namespace Test 
{
	using std::tr1::shared_ptr;

	class TestRunnable : public IRunnable
	{
	public:
		TestRunnable()
			: IsStarted(FALSE), IsRunned(FALSE), IsStopped(FALSE)
		{

		}
		~TestRunnable()
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

	TEST(Win32CurrentThread, Thread) 
	{
		std::tr1::shared_ptr<Thread> currentThread = Thread::CurrentThread();


		EXPECT_FALSE(NULL == currentThread);
	}

	TEST(ThreadTest, Thread)
	{

		TestRunnable* runner1 = new TestRunnable();
		TestRunnable* runner2 = new TestRunnable();

		shared_ptr<Thread> testThread1
			= Thread::CreateThread(shared_ptr<TestRunnable>(runner1), "TestThread" );

		shared_ptr<Thread> testThread2
			= Thread::CreateThread(shared_ptr<TestRunnable>(runner2), "TestThread" );


		EXPECT_TRUE( NULL !=  testThread1);
		EXPECT_TRUE(testThread1->IsCreated());

		EXPECT_TRUE( NULL !=  testThread2);
		EXPECT_TRUE(testThread2->IsCreated());

		testThread1->Join();
		testThread2->Join();

		EXPECT_FALSE(runner1->CurrentThread == runner2->CurrentThread);
		EXPECT_TRUE(runner1->CurrentThread == testThread1);
		EXPECT_TRUE(runner2->CurrentThread == testThread2);


	}

	TEST(MultipleThreadTest, Thread)
	{

		shared_ptr<Thread> testThread1
			= Thread::CreateThread(shared_ptr<TestRunnable>(new TestRunnable()), "TestThread1" );


		EXPECT_TRUE( NULL !=  testThread1);
		EXPECT_TRUE(testThread1->IsCreated());


		shared_ptr<Thread> testThread2
			= Thread::CreateThread(shared_ptr<TestRunnable>(new TestRunnable()), "TestThread2" );


		EXPECT_TRUE( NULL !=  testThread2);
		EXPECT_TRUE(testThread2->IsCreated());


		shared_ptr<Thread> testThread3
			= Thread::CreateThread(shared_ptr<TestRunnable>(new TestRunnable()), "TestThread3" );


		EXPECT_TRUE( NULL !=  testThread3);
		EXPECT_TRUE(testThread3->IsCreated());


		testThread1->Join();
		testThread2->Join();
		testThread3->Join();
	}

}
}
