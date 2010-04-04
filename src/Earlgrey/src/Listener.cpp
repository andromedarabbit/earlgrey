#include "StdAfx.h"
#include "Listener.h"
#include "Executor.h"
#include "Connection.h"

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

	void Listener::DoTask()
	{
		// Accept 이벤트가 왔으므로 처리한다

		// 이벤트를 다시 받을 수 있도록 핸들을 리셋시킨다.
		WSAResetEvent( _AcceptEvent );

		// Accept 처리는 IOCP 스레드에서 하도록 한다.
		IocpExecutorSingleton::Instance().Execute(
			RunnableBuilder::NewRunnable(new AcceptCompleteHandler(_Socket.GetHandle()))
			);		
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

		Connection<DefaultReader, DefaultWriter>* connection = new Connection<DefaultReader, DefaultWriter>();
		connection->Initialize( socket );

		return EXIT_SUCCESS;
	}
}