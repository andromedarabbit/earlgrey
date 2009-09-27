// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerInit.h"
#include "AppInitializer.h"

using namespace Earlgrey;

class WindowsRunnable 
	: public IRunnable
{
public:
	WindowsRunnable() {}
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
		printf("Server Socket = %d\r\n", Socket());
		Stream().AsyncRead();
	}

	virtual void Disconnected()
	{
		printf("Server Disconnected %d", Socket());
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	AppInfo app;
	app.InitInstance(AppType::E_APPTYPE_DEFAULT);

	//ServerCreate(100);
	Acceptor<ServerConnection>* acceptor = new Acceptor<ServerConnection>(100);
	acceptor->Initialize();

	std::tr1::shared_ptr<Thread> serverThread = Thread::CreateRunningThread( 
		std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new WindowsRunnable())), "WindowsRunnable" );

	serverThread->Join();
	
	return EXIT_SUCCESS;
}
