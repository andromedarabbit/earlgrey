#include "stdafx.h"
#include "Connector.h"
#include "Connection.h"
#include "Dns.h"


namespace Earlgrey
{
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

	bool Connector::DoTask()
	{
		WSANETWORKEVENTS Events = {0};

		::WSAEnumNetworkEvents( _Socket.GetHandle(), _WaitEvent, &Events );

		EARLGREY_ASSERT( Events.lNetworkEvents & FD_CONNECT );

		if (Events.iErrorCode[FD_CONNECT_BIT])
		{
			LoggerSingleton::Instance().Debug( Log::ErrorMessage( Events.iErrorCode[FD_CONNECT_BIT] ) );

			if (_RetryCount > 0)
			{
				Sleep( _RetryInterval );
				_Socket.Close();
				ReConnect();
			}
			return true;
		}

		std::tr1::shared_ptr<BinaryConnection> connection(new BinaryConnection());
		connection->Initialize( _Socket );

		// TODO: 어딘가에 Connection 객체를 등록해야 함

		// 이벤트 핸들을 삭제하기 위해 true를 리턴한다.
		return true;
	}
}