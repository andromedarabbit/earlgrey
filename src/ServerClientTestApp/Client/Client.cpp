// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include "DefaultAppSettings.h"
#include "ThreadRunnable.h"
#include "Connector.h"

using namespace Earlgrey;

class WindowsRunnable 
	: public ThreadRunnable
{
public:
	explicit WindowsRunnable()
		: m_stop(FALSE)
	{

	}
	virtual ~WindowsRunnable() {}

protected:
	virtual BOOL Init() { return TRUE; }
	virtual void Stop() {}
	virtual void Exit() {}

	virtual BOOL MeetsStopCondition() const
	{
		return !m_stop;
	}

	virtual DWORD DoTask()
	{
		MSG msg;
		if(!GetMessage(&msg, NULL, 0, 0))
			return EXIT_FAILURE;
		return EXIT_SUCCESS;
	}

private:
	BOOL m_stop;
};

int _tmain(int argc, _TCHAR* argv[])
{
	DBG_UNREFERENCED_PARAMETER(argc);
	DBG_UNREFERENCED_PARAMETER(argv);

	DefaultAppSettings settings;

	Application app(settings);
	EARLGREY_VERIFY(app.InitInstance());//юс╫ц

	Earlgrey::Connector* connector = new Earlgrey::Connector();
	EARLGREY_VERIFY( connector->Connect( "localhost", 100 ) );

	std::tr1::shared_ptr<Thread> WinThread = Thread::CreateThread( 
		std::tr1::shared_ptr<ThreadRunnable>(static_cast<ThreadRunnable*>(new WindowsRunnable()))
		, "WindowsRunnable"
		, WIN_MAIN_THREAD_ID 
		);

	WinThread->Join();

	return EXIT_SUCCESS;
}
