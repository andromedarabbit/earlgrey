#include "stdafx.h"
#include "Socket.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{

	BOOL AsyncStream::Open(SOCKET Socket, CompletionHandler* InHandler)
	{
		Handle = Socket;
		Handler = InHandler;

		INT Zero = 0; 
		setsockopt(Handle, SOL_SOCKET, SO_RCVBUF, (const char*)&Zero, sizeof(Zero));

		Zero = 0;
		setsockopt(Handle, SOL_SOCKET, SO_SNDBUF, (const char*)&Zero, sizeof(Zero));

		//_PacketBuffer->Initialize(); //! todo

		if (!ProactorSingleton::Instance().RegisterHandler( (HANDLE)Handle, InHandler))
		{
			if (Handle != INVALID_SOCKET)
			{
				closesocket(Handle);
			}
			return FALSE;
		}

		return AsyncRead();
	}

	void AsyncStream::Close()
	{
		SOCKET OldSocket = (SOCKET)AtomicExch(Handle, INVALID_SOCKET);

		LINGER Linger;
		Linger.l_onoff = 1;
		Linger.l_linger = 0;
		setsockopt(OldSocket, SOL_SOCKET, SO_LINGER, (const char*)&Linger, sizeof(Linger));
		closesocket(OldSocket);

		//_PacketBuffer->
	}


	BOOL AsyncStream::AsyncRead()
	{
		WSABUF* SocketBuffers = 0; //_PacketBuffer->GetSockRecvBuffer()
		OVERLAPPED* Overlapped = new AsyncReadResult(Handler);
		DWORD ReceivedBytes;
		DWORD Flags = 0;

		int ret=WSARecv(Handle, SocketBuffers, 1,
			&ReceivedBytes, &Flags, Overlapped, NULL);

		if(SOCKET_ERROR == ret)
		{
			int ErrCode = WSAGetLastError();
			if(ErrCode != WSA_IO_PENDING)
			{			
				return FALSE;
			}
		}

		return TRUE;
	}
	
	BOOL AsyncStream::AsyncWrite()
	{
		WSABUF*	SocketBuffer = 0;//= _PacketBuffer->GetSendBuffer(); //! todo
		DWORD	SentBytes;		
		OVERLAPPED* Overlapped = new AsyncWriteResult(Handler);

		INT Error = ::WSASend(Handle, SocketBuffer, _PacketBuffer->GetBufferNum(),
			&SentBytes, 0, Overlapped, NULL);

		if (Error != 0) 
		{ 
			if (Error != WSA_IO_PENDING) 
			{
				if (Error == WSAECONNRESET || Error == WSAEINVAL)
				{
				}
				else
				{
				}
				return FALSE;
			} 
		}
		return TRUE;
	}


	BOOL SocketSubsystem::InitializeSubSystem()
	{
		// MSDN 라이브러리의 예제를 참고했다.
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		wVersionRequested = MAKEWORD( 2, 2 );

		err = WSAStartup( wVersionRequested, &wsaData );
		if ( err != 0 ) {
			// Tell the user that we could not find a usable Winsock DLL.
			// 소켓 시스템 초기화가 실패했으니 WSAGetLastError 도 동작하지 않는다.
			// \todo 오류 처리
			switch(err)
			{
			case WSASYSNOTREADY: // The underlying network subsystem is not ready for network communication.
				break;

			case WSAVERNOTSUPPORTED: // The version of Windows Sockets support requested is not provided by this particular Windows Sockets implementation.
				break;

			case WSAEINPROGRESS: // A blocking Windows Sockets 1.1 operation is in progress.
				break;

			case WSAEPROCLIM: // A limit on the number of tasks supported by the Windows Sockets implementation has been reached.
				break;

			case WSAEFAULT: // The lpWSAData parameter is not a valid pointer.
				break;

			default:
				// \todo 오류 메시지를 넣는 ASSERT, VERIFY가 있으면 좋겠다.
				EARLGREY_VERIFY(TRUE);
				break;
			}

			return FALSE;
		}

		/* Confirm that the WinSock DLL supports 2.2.*/
		/* Note that if the DLL supports versions later    */
		/* than 2.2 in addition to 2.2, it will still return */
		/* 2.2 in wVersion since that is the version we      */
		/* requested.                                        */

		if ( LOBYTE( wsaData.wVersion ) != 2 ||
			HIBYTE( wsaData.wVersion ) != 2 ) {
				/* Tell the user that we could not find a usable */
				/* WinSock DLL.                                  */
				WSACleanup( );
				return FALSE; 
		}

		return TRUE;
	}
}