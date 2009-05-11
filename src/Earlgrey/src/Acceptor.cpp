#include "stdafx.h"
#include "Acceptor.h"
#include "Socket.h"

namespace Earlgrey
{
	BOOL Acceptor::Initialize()
	{
		if( AcceptorSocket != INVALID_SOCKET )
		{
			return FALSE;
		}

		AcceptorSocket = WSASocket(AF_INET, 
			SOCK_STREAM, 
			0, 
			NULL, 
			0, 
			WSA_FLAG_OVERLAPPED);

		if (AcceptorSocket == INVALID_SOCKET)
		{
			return FALSE;
		}

		SOCKADDR_IN Address;
		Address.sin_family = AF_INET;
		Address.sin_addr.s_addr = htonl(INADDR_ANY);
		Address.sin_port = htons((u_short)Port);

		BOOL OptionValue = TRUE;
		setsockopt(AcceptorSocket, 
			SOL_SOCKET, 
			SO_EXCLUSIVEADDRUSE, 
			(const char*)&OptionValue, 
			sizeof(OptionValue));

		if (!bind(AcceptorSocket, (const struct sockaddr*) &Address, sizeof(Address)))
		{
			return FALSE;
		}

		if (listen(AcceptorSocket, SOMAXCONN) == SOCKET_ERROR)
		{
			return FALSE;
		}

		if (AcceptorSocket == INVALID_SOCKET)
		{
			closesocket(AcceptorSocket);
			return FALSE;
		}

		Stream.Open(AcceptorSocket, this);

		HANDLE AcceptEvent = WSACreateEvent();
		//! TODO : Window Event Handler 위한 Thread 로 던지기
		WSAEventSelect(AcceptorSocket, AcceptEvent, FD_ACCEPT);

		return TRUE;
	}

	
	void Acceptor::EventHandler(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* /*InOverlapped*/)
	{
		EARLGREY_ASSERT(Type == READ_EVENT);
		EARLGREY_ASSERT(Handle == (HANDLE)AcceptorSocket);

		SOCKADDR_IN ClientAddress;
		INT ClientAddressSize;

		SOCKET ClientSocket = accept(AcceptorSocket,
			(LPSOCKADDR)&ClientAddress,
			&ClientAddressSize);

		EARLGREY_ASSERT(ClientSocket);

		if (ClientSocket == INVALID_SOCKET)
		{
			//log
			return;
		}

		ConnectionHandler* Conn = new ConnectionHandler((HANDLE)ClientSocket);
		Conn;

		return;
	}

	BOOL ConnectionHandler::Initialize()
	{
		Stream.Open(ClientSocket, this);
		return TRUE;
	}

	void ConnectionHandler::EventHandler(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* /*InOverlapped*/)
	{
		EARLGREY_ASSERT(Handle == (HANDLE)ClientSocket);
		switch(Type)
		{
		case READ_EVENT:
			//PacketHandler(InOverlapped);
			break;
		case WRITE_EVENT:
			Stream.AsyncRead();
			break;
		}
	}
}