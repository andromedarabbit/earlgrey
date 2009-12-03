#include "stdafx.h"
#include "Thread.h"
#include "shared_ptr_helper.h"

namespace Earlgrey { namespace Test 
{
	using std::tr1::shared_ptr;

	class TestServerRunnable : public IRunnable
	{
	public:
		TestServerRunnable()
			: IsStarted(FALSE), IsRunned(FALSE), IsStopped(FALSE)
		{

		}
		~TestServerRunnable()
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

	TEST(ServerTest, Initialize)
	{
	}


}}