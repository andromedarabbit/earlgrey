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
	//	/* Accept된 소켓의 Context를 Update시킨다. (대기소켓에서 활성 소켓으로 변환) */
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

	//	/* 접속된 클라이언트 정보를 받는다. */
	//	GetAcceptExSockaddrs( Buffer->GetAcceptBuffer(), Buffer->GetBufferCapacity(), sizeof(SOCKADDR_STORAGE) + 16, 
	//		sizeof(SOCKADDR_STORAGE) + 16, &LAddr, &LLen, &RAddr, &RLen); 
	//	{ 
	//		//TODO : 아이디어가 필요해~ 사용자가 정의한 connection클래스를 생성해줘야하는데... 이걸 템플릿으로 받자니 죄다 클래스를 다 바꿔줘야하네. 꼭 그래야하나? --; 좀 깔끔하게 해결될 방법 없을까?
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

	//	// AcceptEx를 안 쓰는 이유에 대한 양환씨의 설명.
	//	/* 
	//		accept 로 사용자처리에 문제가 생길까봐 
	//		accept 는 따로 wait thread 가 iocp 와 분리되어 작업하려고 이런 구조를 사용했습니다. 
	//		그래서 AcceptEX 를 사용하지 않음.. 64개라서 좀 문제가 될 수 있는게 빈번하게 접속되고 종료되는 경우 connection timeout이 종종 발생.. 
	//		backlog 설정해주고 wait thread 는 accept만 하므로 크게 문제는 안되지만..
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