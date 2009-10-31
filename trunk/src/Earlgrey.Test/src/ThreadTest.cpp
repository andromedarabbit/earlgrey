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
		BOOL IsStarted;
		BOOL IsRunned;
		BOOL IsStopped;
	};

	TEST(ThreadTest, Thread)
	{

		shared_ptr<Thread> testThread
			= Thread::CreateThread(shared_ptr<TestRunnable>(new TestRunnable()), "TestThread" );


		EXPECT_TRUE( NULL !=  testThread);
		EXPECT_TRUE(testThread->IsCreated());

		testThread->Join();
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
