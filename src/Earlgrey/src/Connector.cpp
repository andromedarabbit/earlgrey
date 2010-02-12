#include "stdafx.h"
#include "ServerInit.h"
#include "Connector.h"



namespace Earlgrey
{

	SOCKET Connector::CreateSocket(const char* RemoteHostName, const INT Port, AsyncStream* InStream)
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

		BOOL OptionValue = TRUE;
		setsockopt(ConnectorSocket, 
			SOL_SOCKET, 
			SO_REUSEADDR, 
			(const char*)&OptionValue, 
			sizeof(OptionValue));//bind WSAEADDRINUSE 오류때문에

		GUID  guid = WSAID_CONNECTEX;
		DWORD bytes = 0;
		LPFN_CONNECTEX lpfnConnectEx;
		if(WSAIoctl(ConnectorSocket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			(LPVOID)&guid, 
			sizeof(guid), 
			(LPVOID)&lpfnConnectEx, 
			sizeof(lpfnConnectEx), 
			&bytes, 
			NULL, 
			NULL) == SOCKET_ERROR)
			return INVALID_SOCKET;

		OVERLAPPED* Overlapped = new AsyncResult(this, InStream);
		if (bind(ConnectorSocket, (LPSOCKADDR)&ServerAddress, sizeof(ServerAddress)) < 0 || 
			(lpfnConnectEx(ConnectorSocket, 
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