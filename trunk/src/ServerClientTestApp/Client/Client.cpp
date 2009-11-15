// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerInit.h"
#include "AppInitializer.h"

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
		return 0;
	}
	virtual void Stop() {}
	virtual void Exit() {}
};

class ClientConnection : public AsyncStream
{
public:
	ClientConnection() 
		: AsyncStream()
	{}
	virtual ~ClientConnection() {}

	virtual void Connected()
	{
		printf("Client Socket = %d\r\n", Socket());
		NetworkBuffer* buf = GetNetworkBuffer();
		BYTE str[13] = "hello server";
		buf->SetValue(&str[0], sizeof(str));
		Send();
	}

	virtual void Disconnected()
	{
		printf("Client Disconnected = %d", Socket());
	}

	virtual void Received()
	{

	}

	virtual void Sent()
	{

	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	DBG_UNREFERENCED_PARAMETER(argc);
	DBG_UNREFERENCED_PARAMETER(argv);

	AppInfo Application;

	EARLGREY_VERIFY(Application.InitInstance(AppType::E_APPTYPE_DEFAULT));

	ClientConnection* connection = new ClientConnection();
	char* ServerIP = "localhost";//! TODO : type and ip
	EARLGREY_ASSERT(connection->Connect(ServerIP, 100) == TRUE);

	std::tr1::shared_ptr<Thread> WinThread = Thread::CreateThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new WindowsRunnable())), "WindowsRunnable" );

	WinThread->Join();

	return EXIT_SUCCESS;
}
