#include "stdafx.h"
#include "Thread.h"

namespace Earlgrey { namespace Test 
{
	class TestRunnable : public IRunnable
	{
	public:
		BOOL Init() { return TRUE; }
		DWORD Run() 
		{
			return 0;
		}

		void Stop() {}
		void Exit() {}
	};

	TEST(ThreadTest, Thread)
	{
		Thread* myThread = new Thread();
		EXPECT_TRUE( myThread->Create( new TestRunnable(), "testRunnable" ) );
	}
}
}
