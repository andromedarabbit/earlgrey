#include "stdafx.h"
#include "Acceptor.h"
#include "Socket.h"

namespace Earlgrey
{
	/**
	*/
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

		AcceptorEvent = WSACreateEvent();
		//! TODO : AcceptorThread¸¸µé±â & AcceptorThread::init(AcceptorEvent, this) 
		WSAEventSelect(AcceptorSocket, AcceptorEvent, FD_ACCEPT);

		return TRUE;
	}

	void Acceptor::HandleEvent()
	{
		Stream.Post();
	}

	void Acceptor::HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* /*InOverlapped*/)
	{
		EARLGREY_ASSERT(Handle == (HANDLE)AcceptorSocket);

		switch(Type)
		{
		case READ_EVENT:
		case WRITE_EVENT://??
			{
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
			}
			break;

		}

		return;
	}


	/**
	*/
	BOOL ConnectionHandler::Initialize()
	{
		Stream.Open(ClientSocket, this);
		Stream.AsyncRead();

		return TRUE;
	}

	void ConnectionHandler::HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* /*InOverlapped*/)
	{
		EARLGREY_ASSERT(Handle == (HANDLE)ClientSocket);
		switch(Type)
		{
		case READ_EVENT:
			//Stream.GetNetworkBuffer()->PacketHandler(InOverlapped);
			break;

		case WRITE_EVENT:
			//write done process
			break;
		}
	}


	/**
	*/
	BOOL AcceptorRunnable::Init()
	{
		return TRUE;
	}

	void AcceptorRunnable::Init(WSAEVENT event, WaitEventHandler* handler)
	{
		Event = event;
		WaitHandler = handler;
	}

	DWORD AcceptorRunnable::Run()
	{
		for(;;)
		{
			DWORD WaitMilliSecond = 50;

			DWORD Index	= WaitForMultipleObjects(
				1,
				&Event,
				FALSE,
				WaitMilliSecond
				);

			if (Index == WAIT_IO_COMPLETION)
			{
				WaitHandler->HandleEvent();
			}
			else if (Index == WAIT_TIMEOUT)
			{		
			}
			else
			{
			}
		}

	}
}