#pragma once 

namespace Earlgrey
{
	class AsyncStream;

	template <class ConnectionType>
	class Connector
		: public WaitEventHandler
	{
	public:
		explicit Connector()
			: ConnectorSocket(INVALID_SOCKET)
		{}
		virtual ~Connector() {}

		void Initialize();

		virtual void HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* Result);
		virtual HANDLE GetHandle() {return (HANDLE)ConnectorSocket;}

		virtual void HandleEvent();

		BOOL Connect(const char* RemoteHostName, const INT Port);
		void Disconnect();
		void Close();

	private:
		SOCKET ConnectorSocket;
		IPAddress ServerAddress;
		WSAEVENT ConnectorEvent;
		AsyncStream Stream;
	};

	template <class ConnectionType>
	inline BOOL Connector<ConnectionType>::Connect(const char* RemoteHostName, const INT Port)
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
		EARLGREY_ASSERT(ConnectorEvent != WSA_INVALID_EVENT);
		INT ret = WSAEventSelect(ConnectorSocket, ConnectorEvent, FD_CONNECT);
		if(ret == SOCKET_ERROR)
		{
			//ret = WSAGetLastError();
			return FALSE;
		}

		AcceptProactorSingleton::Instance().RegisterHandler(ConnectorEvent, this);

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

	template <class ConnectionType>
	inline void Connector<ConnectionType>::Disconnect()
	{
		Close();
	}

	template <class ConnectionType>
	inline void Connector<ConnectionType>::Close()
	{
		if(ConnectorSocket != INVALID_SOCKET)
		{
			closesocket(ConnectorSocket);
			ConnectorSocket = INVALID_SOCKET;
		}
	}

	template <class ConnectionType>
	void Connector<ConnectionType>::HandleEvent()
	{
		//lock?

		WSANETWORKEVENTS	Events;	
		BOOL				Result = TRUE;

		INT ret = WSAEnumNetworkEvents(ConnectorSocket, ConnectorEvent, &Events);
		if ( ret == SOCKET_ERROR )
		{
			Result = FALSE;
		}

		if (Result && !(Events.lNetworkEvents & FD_CONNECT))
		{
			Result= FALSE;
		}

		if (Result && Events.iErrorCode[FD_CONNECT_BIT] != 0)
		{
			Result= FALSE;
		}

		//	UnregisterWait !!!		FConnect wait once !
		if (Result && ConnectorEvent != WSA_INVALID_EVENT)
		{
			AcceptProactorSingleton::Instance().DeregisterHandler(ConnectorEvent);
			WSACloseEvent(ConnectorEvent);
			ConnectorEvent = WSA_INVALID_EVENT;
		}

		if (Result)
		{
			//	Connect success	
			ConnectionHandler* connection = new ConnectionType((HANDLE)ConnectorSocket);
			connection->Connected();
			if (!connection)
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

	template <class ConnectionType>
	inline void Connector<ConnectionType>::HandleEvent(HANDLE /*Handle*/, IOCP_EVENT_TYPE Type, AsyncResult* /*Result*/)
	{
		switch(Type)
		{
		case READ_EVENT:
		case WRITE_EVENT:
			break;
		}
	}
}