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
			// Accept �̺�Ʈ�� ����Ѵ�.
			_AcceptEvent = WSACreateEvent();
			WSAEventSelect( _Socket.GetHandle(), _AcceptEvent, FD_ACCEPT );
			WaitEventContainerSingleton::Instance().Add( _AcceptEvent, this );
		}

		return TRUE;
	}

	void Listener::DoTask()
	{
		// Accept �̺�Ʈ�� �����Ƿ� ó���Ѵ�

		// �̺�Ʈ�� �ٽ� ���� �� �ֵ��� �ڵ��� ���½�Ų��.
		WSAResetEvent( _AcceptEvent );

		// Accept ó���� IOCP �����忡�� �ϵ��� �Ѵ�.
		IocpExecutorSingleton::Instance().Execute(
			RunnableBuilder::NewRunnable(new AcceptCompleteHandler(_Socket.GetHandle()))
			);		
	}

	DWORD AcceptCompleteHandler::Run()
	{
		// �� ��ƾ�� IOCP �����忡�� ����ȴ�.

		SOCKADDR_IN ClientAddress;
		INT ClientAddressSize = sizeof(ClientAddress);
		SOCKET ClientSocket = accept( _ListenSocket.GetHandle(), (struct sockaddr*)&ClientAddress, &ClientAddressSize);
		if (ClientSocket == INVALID_SOCKET)
		{
			// TODO: ����ó��
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