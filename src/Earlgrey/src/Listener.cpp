#include "StdAfx.h"
#include "Listener.h"
#include "Executor.h"
#include "Connection.h"
#include "shared_ptr_helper.h"
#include "INetEvent.h"

#pragma comment(lib, "ws2_32.lib")

namespace Earlgrey {

	Listener::Listener(void)
		: _Socket(INVALID_SOCKET)
		, _AcceptEvent(NULL)
		, _ExclusiveAddressUse(false)
		, m_Active(false)
	{
	}

	Listener::~Listener(void)
	{
		_Socket.Close();
	}

	void Listener::ExclusiveAddressUse(bool exclusiveAddressUse)
	{
		if(m_Active)
		{
			//  throw new InvalidOperationException(SR.GetString("net_tcplistener_mustbestopped"));
			throw new std::exception("");
		}
		_ExclusiveAddressUse = exclusiveAddressUse;
	}

	BOOL Listener::Listen( const IPEndPoint& localEP ) // USHORT Port )
	{
		if (_Socket.IsValid())
		{
			return FALSE;
		}

// 		if (!_Socket.CreateTcpSocket())
// 		{
// 			return FALSE;
// 		}

		// if (!_Socket.Bind( localEP.Port() ))
		if (!_Socket.Bind(localEP) )
		{
			_Socket.Close();
			return FALSE;
		}

// 		if (_ExclusiveAddressUse)
// 		{
// 			_Socket.SetExclusiveAddressUse();
// 		}
// 		else
// 		{
// 			_Socket.SetReuseAddress();
// 		}

		if (!_Socket.Listen())
		{
			_Socket.Close();
			return FALSE;
		}

		if( 0 != localEP.Port() )
		{
			// Accept 이벤트를 등록한다.
			_AcceptEvent = WSACreateEvent();
			WSAEventSelect( _Socket.GetHandle(), _AcceptEvent, FD_ACCEPT );
			WaitEventContainerSingleton::Instance().Add( _AcceptEvent, this );
		}

		m_Active = true;
		return TRUE;
	}

	bool Listener::DoTask()
	{
		EARLGREY_ASSERT( _NetEvent.get() );
		EARLGREY_ASSERT( _PacketHandler.get() );

		// Accept 이벤트가 왔으므로 처리한다

		// 이벤트를 다시 받을 수 있도록 핸들을 리셋시킨다.
		WSAResetEvent( _AcceptEvent );

		// Accept 처리는 IOCP 스레드에서 하도록 한다.
		IocpExecutorSingleton::Instance().Execute(
			RunnableBuilder::NewRunnable(new AcceptCompleteHandler(_Socket.GetHandle(), _NetEvent, _PacketHandler))
			);		

		// 핸들을 계속 유지해야 하므로 false를 리턴한다.
		return false;
	}

	void Listener::Initialize( std::tr1::shared_ptr<INetEvent> NetEvent, std::tr1::shared_ptr<IPacketHandler> PacketHandler )
	{
		_PacketHandler = PacketHandler;
		_NetEvent = NetEvent;
	}

	DWORD AcceptCompleteHandler::Run()
	{
		// 이 루틴은 IOCP 스레드에서 수행된다.

		SOCKADDR_IN ClientAddress;
		INT ClientAddressSize = sizeof(ClientAddress);
		SOCKET ClientSocket = accept( _ListenSocket.GetHandle(), (struct sockaddr*)&ClientAddress, &ClientAddressSize);
		if (ClientSocket == INVALID_SOCKET)
		{
			// TODO: 에러처리
			return EXIT_SUCCESS;
		}

		Socket socket(ClientSocket);
		
		socket.SetReceiveBufferSize( 0 );
		socket.SetSendBufferSize( 0 );

		std::tr1::shared_ptr<Connection> connection = make_ptr(new (alloc<Connection>()) Connection());
		connection->Initialize( socket, _NetEvent, _PacketHandler );

		_NetEvent->OnConnected( connection );

		return EXIT_SUCCESS;
	}

	AcceptCompleteHandler::AcceptCompleteHandler( 
		SOCKET ListenSocket, 
		std::tr1::shared_ptr<INetEvent> NetEvent, 
		std::tr1::shared_ptr<IPacketHandler> PacketHandler ) 
		: _ListenSocket( ListenSocket ), _NetEvent(NetEvent), _PacketHandler(PacketHandler)
	{
		EARLGREY_ASSERT(ListenSocket != INVALID_SOCKET);
		EARLGREY_ASSERT( _NetEvent.get() );
		EARLGREY_ASSERT( _PacketHandler.get() );
	}

}