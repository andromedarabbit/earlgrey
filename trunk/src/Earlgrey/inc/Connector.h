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
		{}
		virtual ~Connector() {}

		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes);
		virtual void HandleEventError(AsyncResult* Result, DWORD Error);

		BOOL Register();
		SOCKET CreateSocket(const char* RemoteHostName, const INT Port, AsyncStream* InStream);

	private:
		void Close()
		{
			if(ConnectorSocket != INVALID_SOCKET)
			{
				closesocket(ConnectorSocket);
				ConnectorSocket = INVALID_SOCKET;
			}
		}
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

	inline SOCKET Connector::CreateSocket(const char* RemoteHostName, const INT Port, AsyncStream* InStream)
	{
		if (ConnectorSocket != INVALID_SOCKET) 
		{
			return INVALID_SOCKET;
		}

		ConnectorSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	

		if (ConnectorSocket==INVALID_SOCKET)
		{
			return INVALID_SOCKET;
		}

		DWORD	On = 1;
		if (ioctlsocket(ConnectorSocket, FIONBIO, &On) == SOCKET_ERROR)
		{
			Close();
			return INVALID_SOCKET;
		}

		HOSTENT* HostEnt = gethostbyname(RemoteHostName);
		if (!HostEnt)
		{
			Close();
			return INVALID_SOCKET;
		}

		if (HostEnt->h_addrtype == PF_INET)
		{
			ServerAddress.SetAddr(*(in_addr*)(*HostEnt->h_addr_list));
		}
		else
		{
			Close();
			return INVALID_SOCKET;
		}

		ServerAddress.SetPort(Port);	

		GUID  guid = WSAID_CONNECTEX;
		DWORD bytes = 0;
		LPFN_CONNECTEX lpfnConnectEx;
		if (!WSAIoctl(ConnectorSocket, SIO_GET_EXTENSION_FUNCTION_POINTER, 
			(LPVOID)&guid, sizeof(guid), (LPVOID)&lpfnConnectEx, 
			sizeof(lpfnConnectEx), &bytes, NULL, NULL))
		{
			closesocket(ConnectorSocket);
		}

		OVERLAPPED* Overlapped = new AsyncResult(this, InStream);
		if ((bind( ConnectorSocket, (LPSOCKADDR)&ServerAddress, sizeof(ServerAddress)) < 0) || 
			(lpfnConnectEx( ConnectorSocket, 
			(LPSOCKADDR)&ServerAddress, 
			sizeof(ServerAddress), 
			NULL, 
			0, 
			NULL, 
			Overlapped) == FALSE))
			switch (WSAGetLastError())
		{
			case ERROR_IO_PENDING: 
				break;
			default :
				return INVALID_SOCKET;
		}

		return ConnectorSocket;

	}
}