#include "stdafx.h"
#include "Thread.h"

namespace Earlgrey { namespace Test 
{
	class TestThread : public IRunnable
	{
	public:
		TestThread()
			: IsStarted(FALSE), IsRunned(FALSE), IsStopped(FALSE)
		{

		}
		BOOL Init() 
		{ 
			IsStarted = TRUE;
			return TRUE; 
		}
		DWORD Run() 
		{
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
		EXPECT_TRUE( NULL != Thread::CreateRunningThread( 
			std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new TestThread())), "TestThread" ) );
		// Need to Join
	}
}
}
