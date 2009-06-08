#pragma once 

namespace Earlgrey
{
	template <class ConnectionType>
	class Acceptor 
		: public WaitEventHandler
	{
	public:
		explicit Acceptor(DWORD InPort)
			: AcceptorSocket(INVALID_SOCKET),
			Port(InPort)
		{};

		virtual ~Acceptor() {};

		BOOL Initialize();
		
		// CompletionHandler Interface
		virtual void HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* Result);
		virtual HANDLE GetHandle() { return (HANDLE)AcceptorSocket; };

		// WaitEventHandler Interface
		virtual void HandleEvent();

	private:
		SOCKET AcceptorSocket;
		WSAEVENT AcceptorEvent;
		AsyncStream Stream;

		DWORD Port;
	};


	/**
	*/
	template <class ConnectionType>
	inline BOOL Acceptor<ConnectionType>::Initialize()
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
			SO_REUSEADDR, 
			(const char*)&OptionValue, 
			sizeof(OptionValue));

		if (bind(AcceptorSocket, (const struct sockaddr*) &Address, sizeof(Address)) == SOCKET_ERROR)
		{
			DWORD a = GetLastError();
			UNREFERENCED_PARAMETER(a);
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

		// AcceptEx를 안 쓰는 이유에 대한 양환씨의 설명.
		/* 
			accept 로 사용자처리에 문제가 생길까봐 
			accept 는 따로 wait thread 가 iocp 와 분리되어 작업하려고 이런 구조를 사용했습니다. 
			그래서 AcceptEX 를 사용하지 않음.. 64개라서 좀 문제가 될 수 있는게 빈번하게 접속되고 종료되는 경우 connection timeout이 종종 발생.. 
			backlog 설정해주고 wait thread 는 accept만 하므로 크게 문제는 안되지만..
			*/
		AcceptorEvent = WSACreateEvent();
		WSAEventSelect(AcceptorSocket, AcceptorEvent, FD_ACCEPT);		
		AcceptProactorSingleton::Instance().RegisterHandler(AcceptorEvent, this);

		return TRUE;
	}

	template <class ConnectionType>
	inline void Acceptor<ConnectionType>::HandleEvent()
	{
		WSANETWORKEVENTS	Events;	
		BOOL				Result = TRUE;

		INT ret = WSAEnumNetworkEvents(AcceptorSocket, AcceptorEvent, &Events);
		if ( ret == SOCKET_ERROR )
		{
			Result = FALSE;
		}

		if (Result && !(Events.lNetworkEvents & FD_ACCEPT))
		{
			Result= FALSE;
		}

		if (Result && Events.iErrorCode[FD_ACCEPT_BIT] != 0)
		{
			Result= FALSE;
		}
		if(Result)
		{
			WSAResetEvent(AcceptorEvent);	
			Stream.Post();
		}
	}

	template <class ConnectionType>
	inline void Acceptor<ConnectionType>::HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* /*InOverlapped*/)
	{
		EARLGREY_ASSERT(Handle == (HANDLE)AcceptorSocket);

		switch(Type)
		{
		case READ_EVENT:
		case WRITE_EVENT://??
			{
				SOCKADDR_IN ClientAddress;
				INT ClientAddressSize = sizeof(ClientAddress);

				SOCKET ClientSocket = accept(AcceptorSocket,
					(LPSOCKADDR)&ClientAddress,
					&ClientAddressSize);

				EARLGREY_ASSERT(ClientSocket);

				if (ClientSocket == INVALID_SOCKET)
				{
					//DWORD a = GetLastError();
					return;
				}

				ConnectionHandler* connection = new ConnectionType((HANDLE)ClientSocket);
				connection->Connected();
			}
			break;

		}

		return;
	}

	class ConnectionHandler 
		: public CompletionHandler
	{
	public:
		explicit ConnectionHandler(HANDLE InHandle)
		{
			Socket = (SOCKET)InHandle;
			Initialize();
		};

		BOOL Initialize();

		virtual ~ConnectionHandler() {};

		// CompletionHandler Interface
		virtual void  HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* Result);
		virtual HANDLE GetHandle() { return (HANDLE)Socket; };

		// 
		virtual void Connected() {}

	public://임시private:
		SOCKET Socket;
		AsyncStream Stream;

		//PacketHandler _PacketHandler;

	};

}