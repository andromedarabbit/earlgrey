#include "StdAfx.h"
#include "..\inc\Listener.h"
#include "Executor.h"
#include "Connection.h"

namespace Earlgrey {

	Listener::Listener(void)
	{
	}

	Listener::~Listener(void)
	{
		_Socket.Close();
	}

	BOOL Listener::Listen( USHORT Port, bool ReuseAddress )
	{
		if (_Socket.IsValid())
		{
			return FALSE;
		}

		if (!_Socket.CreateTcpSocket())
		{
			return FALSE;
		}

		if (ReuseAddress)
		{
			_Socket.SetReuseAddress();
		}
		else
		{
			_Socket.SetExclusiveAddressUse();
		}

		if (!_Socket.Bind( Port ))
		{
			_Socket.Close();
			return FALSE;
		}

		if (!_Socket.Listen())
		{
			_Socket.Close();
			return FALSE;
		}

		if( 0 != Port )
		{
			// Accept 이벤트를 등록한다.
			_AcceptEvent = WSACreateEvent();
			WSAEventSelect( _Socket.GetHandle(), _AcceptEvent, FD_ACCEPT );
			WaitEventContainerSingleton::Instance().Add( _AcceptEvent, this );
		}

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
			return 0;
		}

		Socket socket;
		socket.Attach( ClientSocket );

		socket.SetReceiveBufferSize( 0 );
		socket.SetSendBufferSize( 0 );

		Connection<DefaultReader, DefaultWriter>* connection = new Connection<DefaultReader, DefaultWriter>();
		connection->Initialize( socket.GetHandle() );

		return 0;
	}
}