#include "stdafx.h"

#include "Connector.h"
#include "Acceptor.h"

namespace Earlgrey
{
	BOOL Connector::Connect(const char* RemoteHostName, const INT Port)
	{
		//Lock??

		if (ConnectorSocket != INVALID_SOCKET) 
		{
			return TRUE;
		}

		ConnectorSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	

		if (ConnectorSocket==INVALID_SOCKET)
		{
			return FALSE;
		}

		DWORD	On = 1;
		if (ioctlsocket(ConnectorSocket, FIONBIO, &On) == SOCKET_ERROR)
		{
			Close();
			return FALSE;
		}

		HOSTENT* HostEnt = gethostbyname(RemoteHostName);
		if (!HostEnt)
		{
			Close();
			return FALSE;
		}

		if (HostEnt->h_addrtype == PF_INET)
		{
			ServerAddress.SetAddr(*(in_addr*)(*HostEnt->h_addr_list));
		}
		else
		{
			Close();
			return FALSE;
		}
		
		ServerAddress.SetPort(Port);	

		ConnectorEvent = WSACreateEvent();
		EARLGREY_ASSERT(ConnectorEvent);
		WSAEventSelect(ConnectorSocket, ConnectorEvent, FD_CONNECT);

		//RegisterWaitEvent(ConnectorEvent, this); acceptor thread 로 던지는 게 맞을까?

		if (connect(ConnectorSocket, (const sockaddr*) &ServerAddress, sizeof(ServerAddress)) == SOCKET_ERROR)
		{
			INT Error = WSAGetLastError();
			if (Error != WSAEWOULDBLOCK)
			{
				Close();
				return FALSE;
			}
		}

		return TRUE;
	}

	void Connector::Disconnect()
	{
		Close();
	}

	void Connector::Close()
	{
		if(ConnectorSocket != INVALID_SOCKET)
		{
			closesocket(ConnectorSocket);
			ConnectorSocket = INVALID_SOCKET;
		}
	}

	void Connector::HandleEvent(HANDLE /*Handle*/, IOCP_EVENT_TYPE /*Type*/, AsyncResult* /*Result*/)
	{

	}
	
	void Connector::HandleEvent()
	{
		//lock?

		WSANETWORKEVENTS	Events;	
		BOOL				Result = TRUE;

		if (WSAEnumNetworkEvents(ConnectorSocket, ConnectorEvent, &Events))
		{
			Result = FALSE;
		}

		//	UnregisterWait !!!		FConnect wait once !
		if (ConnectorEvent != WSA_INVALID_EVENT)
		{
			DeregisterWaitEvent(ConnectorEvent);
			ConnectorEvent = WSA_INVALID_EVENT;
		}

		if (Result)
		{
			//	Connect success	
			ConnectionHandler* Conn = new ConnectionHandler((HANDLE)ConnectorSocket);
			Conn;
			if (!Conn)
			{
				Close();
			}
			else 
			{
				ConnectorSocket = INVALID_SOCKET;
			}
		}
		else
		{
			Close();
		}
	}
}