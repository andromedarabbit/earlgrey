// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerInit.h"

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

class ServerConnection : public ConnectionHandler
{
public:
	ServerConnection (HANDLE InHandle) 
		: ConnectionHandler(InHandle)
	{}
	virtual ~ServerConnection () {}

	virtual void Connected()
	{
		Stream.AsyncRead();
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Init();

	//ServerCreate(100);
	Acceptor<ServerConnection>* acceptor = new Acceptor<ServerConnection>(100);
	acceptor->Initialize();

	ServerCreated();

	Thread* WinThread = Thread::CreateRunningThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new WindowsRunnable())), "WindowsRunnable" );

	WaitForSingleObject(WinThread->GetWindowHandle(), INFINITE );
	
	return EXIT_SUCCESS;
}
