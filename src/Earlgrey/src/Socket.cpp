#include "stdafx.h"
#include "Socket.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	BOOL SocketInterface::Initialize()
	{
		INT ZeroInt; 
		ZeroInt = 0;
		setsockopt(_Handle, SOL_SOCKET, SO_RCVBUF, (const char*)&ZeroInt, sizeof(ZeroInt));

		ZeroInt = 0;
		setsockopt(_Handle, SOL_SOCKET, SO_SNDBUF, (const char*)&ZeroInt, sizeof(ZeroInt));

		if (!GProactor.RegisterHandler( (HANDLE)_Handle, static_cast<void*>(this)))
		{
			Close();
			return FALSE;
		}

		//_PacketBuffer->Initialize();
		return TRUE;
	}

	void SocketInterface::Close()
	{
		SOCKET OldSocket = (SOCKET)AtomicExch(_Handle, INVALID_SOCKET);

		LINGER Linger;
		Linger.l_onoff = 1;
		Linger.l_linger = 0;
		setsockopt(OldSocket, SOL_SOCKET, SO_LINGER, (const char*)&Linger, sizeof(Linger));
		closesocket(OldSocket);
	}

	BOOL SocketInterface::Send()
	{
		WSABUF*	SocketBuffer = 0;//= _PacketBuffer->GetSendBuffer(); //! todo
		DWORD	SentBytes;		
		INT Error = ::WSASend(	
			_Handle,		//	Socket Handle
			SocketBuffer,		//	Buffer
			_PacketBuffer->GetBufferNum(),	//	Buffer Count
			&SentBytes,			//	Sent Bytes
			0,					//	Flag
			&_OverlappedSend,		//	Overlapped Pointer
			NULL
			);

		if (Error != 0) 
		{ 
			if (Error != ERROR_IO_PENDING) 
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

	BOOL SocketInterface::Receive()
	{
		WSABUF* SocketBuffers = 0; //_PacketBuffer->GetSockRecvBuffer()
		DWORD ReceivedBytes;
		DWORD Flags = 0;

		INT ReceiveResult = ::WSARecv(
			_Handle,
			SocketBuffers,
			1,
			&ReceivedBytes,
			&Flags,
			&_OverlappedRead,
			NULL);

		if (ReceiveResult != 0)
		{
			INT Error = GetLastError();
			if(Error != ERROR_IO_PENDING)
			{			
				return Error;
			}
		}
		return 0;

	}

	void SocketInterface::IODone(BOOL InSuccess, DWORD InTransferred, LPOVERLAPPED InOverlapped)
	{
		if (!InSuccess)
		{
			if (InOverlapped == &_OverlappedRead)	
				Close();		
		}
		else if (InOverlapped == &_OverlappedRead)
		{
			//	Receive IO Completed
			ReceiveCompleted(InTransferred);
		}
		else if (InOverlapped == &_OverlappedSend)
		{
			//	Send IO Completed
			SendCompleted(InTransferred);
		}
		else if (InOverlapped == NULL)
		{
		}
		else
		{
		}
	}

	void SocketInterface::SendCompleted(DWORD )
	{
	}

	void SocketInterface::ReceiveCompleted(DWORD )
	{
		OnReceived();
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