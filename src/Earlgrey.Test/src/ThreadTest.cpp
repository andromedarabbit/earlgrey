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
		EXPECT_TRUE( NULL != Thread::CreateRunningThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new TestRunnable())), "testRunnable" ) );
	}
}
}
