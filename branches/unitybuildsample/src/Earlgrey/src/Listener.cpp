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
			// Accept �̺�Ʈ�� ����Ѵ�.
			_AcceptEvent = WSACreateEvent();
			WSAEventSelect( _Socket.GetHandle(), _AcceptEvent, FD_ACCEPT );
			WaitEventContainerSingleton::Instance().Add( _AcceptEvent, this );
		}

		m_Active = true;
		return TRUE;
	}

	bool Listener::DoTask()
	{
		// Accept �̺�Ʈ�� �����Ƿ� ó���Ѵ�

		// �̺�Ʈ�� �ٽ� ���� �� �ֵ��� �ڵ��� ���½�Ų��.
		WSAResetEvent( _AcceptEvent );

		// Accept ó���� IOCP �����忡�� �ϵ��� �Ѵ�.
		IocpExecutorSingleton::Instance().Execute(
			RunnableBuilder::NewRunnable(new AcceptCompleteHandler(_Socket.GetHandle()))
			);		

		// �ڵ��� ��� �����ؾ� �ϹǷ� false�� �����Ѵ�.
		return false;
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