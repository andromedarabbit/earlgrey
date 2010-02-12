#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"

namespace Earlgrey
{
	class Connector
		: public CompletionHandler
	{
	public:
		explicit Connector()
			: ConnectorSocket(INVALID_SOCKET)
		{

		}

		virtual ~Connector() 
		{

		}

		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes);
		virtual void HandleEventError(AsyncResult* Result, DWORD Error);

		BOOL Register();
		SOCKET CreateSocket(const char* RemoteHostName, const INT Port, AsyncStream* InStream);

	private:
		void Close();

	private:
		SOCKET ConnectorSocket;
		IPAddress ServerAddress;
	};

	inline BOOL Connector::Register()
	{
		if (!ProactorSingleton::Instance().RegisterHandler( (HANDLE)ConnectorSocket, this))
		{
			return FALSE;
		}

		return TRUE;
	}

	inline void Connector::HandleEvent(AsyncResult* Result, DWORD /*TransferredBytes*/)
	{
		//lock?

		if ((Result->Error() == (DWORD)-1) ||
			(setsockopt( ConnectorSocket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0) < 0))
		{
			return;
		}
		else 
		{
			Result->Stream()->Connected();
		}
	}

	inline void Connector::HandleEventError(AsyncResult* /*Result*/, DWORD /*Error*/)
	{

	}


	inline void Connector::Close()
	{
		if(ConnectorSocket != INVALID_SOCKET)
		{
			closesocket(ConnectorSocket);
			ConnectorSocket = INVALID_SOCKET;
		}
	}

}