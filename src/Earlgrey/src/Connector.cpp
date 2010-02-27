#include "stdafx.h"
#include "ServerInit.h"
#include "Connector.h"



namespace Earlgrey
{

	//SOCKET Connector::CreateSocket(const char* RemoteHostName, const INT Port, AsyncStream* /*InStream*/)
	//{
	//	if (ConnectorSocket != INVALID_SOCKET) 
	//	{
	//		return INVALID_SOCKET;
	//	}

	//	ConnectorSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	

	//	if (ConnectorSocket==INVALID_SOCKET)
	//	{
	//		return INVALID_SOCKET;
	//	}

	//	DWORD	On = 1;
	//	if (ioctlsocket(ConnectorSocket, FIONBIO, &On) == SOCKET_ERROR)
	//	{
	//		Close();
	//		return INVALID_SOCKET;
	//	}

	//	HOSTENT* HostEnt = gethostbyname(RemoteHostName);
	//	if (!HostEnt)
	//	{
	//		Close();
	//		return INVALID_SOCKET;
	//	}

	//	if (HostEnt->h_addrtype == PF_INET)
	//	{
	//		ServerAddress.SetAddr(*(in_addr*)(*HostEnt->h_addr_list));
	//	}
	//	else
	//	{
	//		Close();
	//		return INVALID_SOCKET;
	//	}

	//	ServerAddress.SetPort(Port);	

	//	BOOL OptionValue = TRUE;
	//	setsockopt(ConnectorSocket, 
	//		SOL_SOCKET, 
	//		SO_REUSEADDR, 
	//		(const char*)&OptionValue, 
	//		sizeof(OptionValue));//bind WSAEADDRINUSE 오류때문에

	//	GUID  guid = WSAID_CONNECTEX;
	//	DWORD bytes = 0;
	//	LPFN_CONNECTEX lpfnConnectEx;
	//	if(WSAIoctl(ConnectorSocket, 
	//		SIO_GET_EXTENSION_FUNCTION_POINTER, 
	//		(LPVOID)&guid, 
	//		sizeof(guid), 
	//		(LPVOID)&lpfnConnectEx, 
	//		sizeof(lpfnConnectEx), 
	//		&bytes, 
	//		NULL, 
	//		NULL) == SOCKET_ERROR)
	//		return INVALID_SOCKET;

	//	OVERLAPPED* Overlapped = (new AsyncResult(ConnectorSocket, this))->GetOverlapped();
	//	if (bind(ConnectorSocket, (LPSOCKADDR)&ServerAddress, sizeof(ServerAddress)) < 0 || 
	//		(lpfnConnectEx(ConnectorSocket, 
	//		(LPSOCKADDR)&ServerAddress, 
	//		sizeof(ServerAddress), 
	//		NULL, 
	//		0, 
	//		NULL, 
	//		Overlapped) == FALSE))
	//		switch (WSAGetLastError())
	//	{
	//		case ERROR_IO_PENDING:
	//			break;
	//		default :
	//			return INVALID_SOCKET;
	//	}

	//	return ConnectorSocket;
	//}

	bool Connector::ReConnect()
	{
		if (_Socket.IsValid())
		{
			return false;
		}

		if (!_Socket.CreateTcpSocket())
		{
			return false;
		}

		if (!_Socket.SetNonBlockingSocket())
		{
			_Socket.Close();
			return false;
		}

		Dns::GetHostByName( _ServerName.c_str(), _ServerAddress );
		_ServerAddress.SetPort( _Port );

		_WaitEvent = WSACreateEvent();
		WSAEventSelect( _Socket.GetHandle(), _WaitEvent, FD_CONNECT );
		WaitEventContainerSingleton::Instance().Add( _WaitEvent, this );

		if (connect(_Socket.GetHandle(), (const sockaddr*)(SOCKADDR_IN*)&_ServerAddress, sizeof(_ServerAddress)) == SOCKET_ERROR)
		{
			INT Error = WSAGetLastError();
			if (Error != WSAEWOULDBLOCK)
			{
				// TODO: 에러처리
				return false;
			}
		}

		return true;
	}

	bool Connector::Connect( LPCSTR Server, INT Port )
	{
		_ServerName = Server;
		_Port = Port;

		return ReConnect();
	}
}