// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerInit.h"
#include "Application.h"

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

	Application Application;

	EARLGREY_VERIFY(Application.InitInstance(AppType::E_APPTYPE_CLIENT));//юс╫ц

	ClientConnection* connection = new ClientConnection();
	char* ServerIP = "localhost";//! TODO : type and ip
	EARLGREY_ASSERT(connection->Connect(ServerIP, 100) == TRUE);

	std::tr1::shared_ptr<Thread> WinThread = Thread::CreateThread( 
		std::tr1::shared_ptr<ThreadRunnable>(static_cast<ThreadRunnable*>(new WindowsRunnable()))
		, "WindowsRunnable"
		, WIN_MAIN_THREAD_ID 
		);

	WinThread->Join();

	return EXIT_SUCCESS;
}
