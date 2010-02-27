#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"

namespace Earlgrey
{
	class Connector : public IWaitHandler
	{
	public:
		explicit Connector()
		{
		}

		virtual ~Connector() 
		{
		}

		bool Connect(LPCSTR Server, INT Port);

		bool ReConnect();

		void DoTask()
		{
		}

		BOOL Register();
		//SOCKET CreateSocket(const char* RemoteHostName, const INT Port, AsyncStream* InStream);

	private:
		void Close();

	private:
		Socket _Socket;
		std::string _ServerName;
		INT _Port;
		HANDLE _WaitEvent;
		IPAddress _ServerAddress;
	};

	//inline BOOL Connector::Register()
	//{
	//	if (!ProactorSingleton::Instance().RegisterHandler( (HANDLE)ConnectorSocket, this))
	//	{
	//		return FALSE;
	//	}

	//	return TRUE;
	//}

	//inline void Connector::HandleEvent(AsyncResult* Result)
	//{
	//	//lock?

	//	if ((Result->GetError() == (DWORD)-1) ||
	//		(setsockopt( ConnectorSocket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0) < 0))
	//	{
	//		return;
	//	}
	//}


	//inline void Connector::Close()
	//{
	//	if(ConnectorSocket != INVALID_SOCKET)
	//	{
	//		closesocket(ConnectorSocket);
	//		ConnectorSocket = INVALID_SOCKET;
	//	}
	//}

}