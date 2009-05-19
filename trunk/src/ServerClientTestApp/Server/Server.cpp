// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ServerInit.h"
#include "Thread.h"

using namespace Earlgrey;

class WindowsRunnable 
	: public IRunnable
{
public:
	explicit WindowsRunnable() {}
	virtual ~WindowsRunnable() {}

	virtual BOOL Init(){return TRUE;}
	virtual DWORD Run()
	{
		MSG msg;
		while (GetMessage(&msg, NULL, 0, 0))
		{

		}
		return msg.wParam;
	}
	virtual void Stop() {}
	virtual void Exit() {}
};

int _tmain(int argc, _TCHAR* argv[])
{
	ServerInit();
	ServerCreate(100);

	ClientCreate(100);

	ServerCreated();

	Thread* WinThread = Thread::CreateRunningThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new WindowsRunnable())), "WindowsRunnable" );

	WaitForSingleObject(WinThread->GetWindowHandle(), INFINITE );
	return 0;
}
