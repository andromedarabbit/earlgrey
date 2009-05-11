#include "stdafx.h"
#include "Server.h"
#include "Socket.h"

namespace Earlgrey
{
	BOOL Server::Create(DWORD InPort)
	{
		Port = InPort;

		if( _ServerSocket != INVALID_SOCKET )
		{
			return FALSE;
		}

		_ServerSocket = WSASocket(AF_INET, 
			SOCK_STREAM, 
			0, 
			NULL, 
			0, 
			WSA_FLAG_OVERLAPPED);

		if (_ServerSocket == INVALID_SOCKET)
		{
			return FALSE;
		}

		SOCKADDR_IN Address;
		Address.sin_family = AF_INET;
		Address.sin_addr.s_addr = htonl(INADDR_ANY);
		Address.sin_port = htons((u_short)Port);

		BOOL OptionValue = TRUE;
		setsockopt(_ServerSocket, 
			SOL_SOCKET, 
			SO_EXCLUSIVEADDRUSE, 
			(const char*)&OptionValue, 
			sizeof(OptionValue));

		if (!bind(_ServerSocket, (const struct sockaddr*) &Address, sizeof(Address)))
		{
			return FALSE;
		}

		if (listen(_ServerSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			return FALSE;
		}

		if (_ServerSocket == INVALID_SOCKET)
		{
			closesocket(_ServerSocket);
			return FALSE;
		}

		HANDLE AcceptEvent = WSACreateEvent();
		//! TODO : Window Event Handler 위한 Thread 로 던지기
		WSAEventSelect(_ServerSocket, AcceptEvent, FD_ACCEPT);

		return TRUE;
	}

	void Server::IODone(BOOL /*InSuccess*/, DWORD /*InTransferred*/, LPOVERLAPPED /*InOverlapped*/)
	{
		SOCKADDR_IN ClientAddress;
		INT ClientAddressSize;

		SOCKET ClientSocket = accept(_ServerSocket,
			(LPSOCKADDR)&ClientAddress,
			&ClientAddressSize);

		//EARLGREY_ASSERT(ClientSocket);

		if (ClientSocket == INVALID_SOCKET)
		{
			//INT Error = WSAGetLastError();
			//log
			return;
		}

		//SocketInterface* Socket = new SocketInterface(ClientSocket);

		return;
	}
}