// Client.cpp : Defines the entry point for the console application.
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

class ClientConnection : public ConnectionHandler
{
public:
	ClientConnection(HANDLE InHandle) 
		: ConnectionHandler(InHandle)
	{}
	virtual ~ClientConnection() {}

	virtual void Connected()
	{
		printf("Socket = %d", Socket);
		NetworkBuffer* buf = Stream.GetNetworkBuffer();
		BYTE str[13] = "hello server";
		buf->SetValue(&str[0], sizeof(str));
		Stream.AsyncWrite();
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	Init();

	//ClientCreate(100);
	Connector<ClientConnection>* c = new Connector<ClientConnection>();
	char* ServerIP = "localhost";//! TODO : type and ip
	c->Connect(ServerIP, 100);

	ServerCreated();

	Thread* WinThread = Thread::CreateRunningThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new WindowsRunnable())), "WindowsRunnable" );

	WaitForSingleObject(WinThread->GetWindowHandle(), INFINITE );

	return EXIT_SUCCESS;
}
