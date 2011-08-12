#include "stdafx.h"
#include "Connector.h"
#include "Connection.h"
#include "Dns.h"
#include "shared_ptr_helper.h"
#include "SocketHelper.h"
#include "Connection.h"
#include "INetEvent.h"
#include "IPacketHandler.h"

namespace Earlgrey
{
	
	bool Connector::DoTask()
	{
		WSANETWORKEVENTS Events = {0};

		::WSAEnumNetworkEvents( _Socket.GetHandle(), _WaitEvent, &Events );

		EARLGREY_ASSERT( Events.lNetworkEvents & FD_CONNECT );

		if (Events.iErrorCode[FD_CONNECT_BIT])
		{
			if (_RetryCount > 0)
			{
				Sleep( _RetryInterval );
				_Socket.Close();
				ReConnect();
			}
			return true;
		}

		std::tr1::shared_ptr<Connection> connection = make_ptr(new Connection());
		connection->Initialize( _Socket, _NetEvent, _PacketHandler );

		_NetEvent->OnConnected( connection );

		// 이벤트 핸들을 삭제하기 위해 true를 리턴한다.
		return true;
	}

	bool Connector::ReConnect()
	{
		if (!SocketHelper::SetUpNonBlockingSocket( _Socket ))
		{
			return false;
		}

		_WaitEvent = WSACreateEvent();
		WSAEventSelect( _Socket.GetHandle(), _WaitEvent, FD_CONNECT );
		WaitEventContainerSingleton::Instance().Add( _WaitEvent, this );

		if (connect(_Socket.GetHandle(), (const sockaddr*)(const SOCKADDR_IN*)&_ServerAddress, sizeof(_ServerAddress)) == SOCKET_ERROR)
		{
			INT Error = WSAGetLastError();
			if (Error != WSAEWOULDBLOCK)
			{
				return false;
			}
		}

		return true;
	}

	void Connector::Initialize(
				std::tr1::shared_ptr<INetEvent> NetEvent, 
				std::tr1::shared_ptr<IPacketHandler> PacketHandler, 
				BYTE RetryCount /*= 3*/, 
				DWORD RetryInterval /*= 3000*/)
	{
		_NetEvent = NetEvent;
		_PacketHandler = PacketHandler;
		_RetryCount = RetryCount;
		_RetryInterval = RetryInterval;
	}

	bool Connector::Connect( LPCSTR Server, INT Port )
	{
		_ServerName = Server;
		_Port = Port;

		SocketHelper::SetAddress( _ServerAddress, Server, Port );

		return ReConnect();
	}

}