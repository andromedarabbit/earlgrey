#include "stdafx.h"

#include "ServerInit.h"
#include "Acceptor.h"
#include "NetworkBuffer.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Mswsock.lib")


namespace Earlgrey
{
	//void Acceptor::HandleEvent(AsyncResult* Result)
	//{
	//	/* Accept�� ������ Context�� Update��Ų��. (�����Ͽ��� Ȱ�� �������� ��ȯ) */
	//	if (Result->GetError() == SOCKET_ERROR 
	//		|| setsockopt( AcceptSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&ListenSocket, sizeof(ListenSocket) ) < 0)
	//	{
	//		return;
	//	}

	//	Socket AcceptSocket;
	//	AcceptSocket.Attach( Result->GetHandle() );

	//	if (Result->GetBytesTransferred() == 0)
	//	{
	//		AcceptSocket.Close();
	//		return;
	//	}

	//	SOCKADDR *LAddr = NULL, *RAddr = NULL;
	//	int       LLen = 0, RLen = 0;
	//	NetworkBuffer* Buffer = Result->Stream()->GetNetworkBuffer();

	//	/* ���ӵ� Ŭ���̾�Ʈ ������ �޴´�. */
	//	GetAcceptExSockaddrs( Buffer->GetAcceptBuffer(), Buffer->GetBufferCapacity(), sizeof(SOCKADDR_STORAGE) + 16, 
	//		sizeof(SOCKADDR_STORAGE) + 16, &LAddr, &LLen, &RAddr, &RLen); 
	//	{ 
	//		//TODO : ���̵� �ʿ���~ ����ڰ� ������ connectionŬ������ ����������ϴµ�... �̰� ���ø����� ���ڴ� �˴� Ŭ������ �� �ٲ�����ϳ�. �� �׷����ϳ�? --; �� ����ϰ� �ذ�� ��� ������?
	//		Result->Stream()->Connected();

	//		Result->Stream()->Recv();
	//	} 

	//	CreateAcceptSocket(Result->Stream());

	//	return;
	//}

	//BOOL Acceptor::Listen(USHORT Port, bool ReuseAddress)
	//{
	//	EARLGREY_ASSERT( _Proactor );
	//	if (!_Proactor)
	//	{
	//		return FALSE;
	//	}

	//	if (_ListenSocket.IsValid())
	//	{
	//		return FALSE;
	//	}

	//	if (!_ListenSocket.CreateAsyncTcpSocket())
	//	{
	//		return FALSE;
	//	}

	//	if (!_Proactor->RegisterHandler( (HANDLE)_ListenSocket.GetHandle(), this ))
	//	{
	//		return FALSE;
	//	}

	//	if (ReuseAddress)
	//	{
	//		_ListenSocket.SetReuseAddress();
	//	}
	//	else
	//	{
	//		_ListenSocket.SetExclusiveAddressUse();
	//	}
	//	
	//	if (!_ListenSocket.Bind( Port ))
	//	{
	//		return FALSE;
	//	}

	//	if (!_ListenSocket.Listen())
	//	{
	//		return FALSE;
	//	}

	//	return TRUE;
	//}

	//SOCKET Acceptor::CreateAcceptSocket(AsyncStream* InStream)
	//{
	//	EARLGREY_ASSERT( _Proactor );
	//	if (!_Proactor)
	//	{
	//		return INVALID_SOCKET;
	//	}

	//	// AcceptEx�� �� ���� ������ ���� ��ȯ���� ����.
	//	/* 
	//		accept �� �����ó���� ������ ������ 
	//		accept �� ���� wait thread �� iocp �� �и��Ǿ� �۾��Ϸ��� �̷� ������ ����߽��ϴ�. 
	//		�׷��� AcceptEX �� ������� ����.. 64���� �� ������ �� �� �ִ°� ����ϰ� ���ӵǰ� ����Ǵ� ��� connection timeout�� ���� �߻�.. 
	//		backlog �������ְ� wait thread �� accept�� �ϹǷ� ũ�� ������ �ȵ�����..
	//	*/

	//	LPFN_ACCEPTEX lpfnAcceptEx = NULL;
	//	GUID GuidAcceptEx = WSAID_ACCEPTEX;
	//	DWORD dwBytes;

	//	//----------------------------------------
	//	// Load the AcceptEx function into memory using WSAIoctl.
	//	// The WSAIoctl function is an extension of the ioctlsocket()
	//	// function that can use overlapped I/O. The function's 3rd
	//	// through 6th parameters are input and output buffers where
	//	// we pass the pointer to our AcceptEx function. This is used
	//	// so that we can call the AcceptEx function directly, rather
	//	// than refer to the Mswsock.lib library.
	//	if(WSAIoctl(_ListenSocket.GetHandle(), 
	//			SIO_GET_EXTENSION_FUNCTION_POINTER, 
	//			&GuidAcceptEx, 
	//			sizeof(GuidAcceptEx),
	//			&lpfnAcceptEx, 
	//			sizeof(lpfnAcceptEx), 
	//			&dwBytes, 
	//			NULL, 
	//			NULL) == SOCKET_ERROR)
	//	{
	//		lpfnAcceptEx = AcceptEx;
	//	}

	//	if (!_AcceptSocket.CreateAsyncTcpSocket()) 
	//	{
	//		return INVALID_SOCKET;
	//	}

	//	AsyncResult* Overlapped = new AsyncResult( _AcceptSocket.GetHandle(), this );
	//	TCHAR buf[1024];
	//	if( lpfnAcceptEx(_ListenSocket.GetHandle(), 
	//		_AcceptSocket.GetHandle(),
	//		buf,
	//		1024-((sizeof(sockaddr_in) + 16) * 2),
	//		sizeof(sockaddr_in) + 16, 
	//		sizeof(sockaddr_in) + 16, 
	//		&dwBytes, 
	//		Overlapped) == FALSE)
	//	{
	//		switch (WSAGetLastError())
	//		{
	//		case ERROR_IO_PENDING:
	//			break;
	//		default : 
	//			return INVALID_SOCKET;
	//		}
	//	}

	//	if (!_Proactor->RegisterHandler( (HANDLE)_AcceptSocket.GetHandle(), this ))
	//	{
	//		return FALSE;
	//	}
	//	return _AcceptSocket.GetHandle();
	//}

	//BOOL Acceptor::SetProactor(Proactor* InProactor)
	//{
	//	_Proactor = InProactor;
	//}
}