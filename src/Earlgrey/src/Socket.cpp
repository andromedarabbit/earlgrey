#include "stdafx.h"

#include "ServerInit.h"
#include "Socket.h"

namespace Earlgrey
{

	BOOL AsyncStream::Open(SOCKET Socket, CompletionHandler* Handler)
	{
		_Handle = Socket;
		_Handler = Handler;

		INT Zero = 0; 
		setsockopt(_Handle, SOL_SOCKET, SO_RCVBUF, (const char*)&Zero, sizeof(Zero));

		Zero = 0;
		setsockopt(_Handle, SOL_SOCKET, SO_SNDBUF, (const char*)&Zero, sizeof(Zero));

		_PacketBuffer = new NetworkBuffer();
		_PacketBuffer->Initialize();

		if (!ProactorSingleton::Instance().RegisterHandler( (HANDLE)_Handle, Handler))
		{
			if (_Handle != INVALID_SOCKET)
			{
				closesocket(_Handle);
			}
			return FALSE;
		}

		return TRUE;
	}

	void AsyncStream::Close()
	{
		SOCKET OldSocket = (SOCKET)AtomicExch(_Handle, INVALID_SOCKET);

		LINGER Linger;
		Linger.l_onoff = 1;
		Linger.l_linger = 0;
		setsockopt(OldSocket, SOL_SOCKET, SO_LINGER, (const char*)&Linger, sizeof(Linger));
		closesocket(OldSocket);

		delete _PacketBuffer;//TODO
	}

	BOOL AsyncStream::Post()
	{
		return ProactorSingleton::Instance().PostEvent( _Handler, new AsyncWriteResult(_Handler) );
	}

	BOOL AsyncStream::AsyncRead()
	{
		WSABUF* SocketBuffers = _PacketBuffer->GetSockRecvBuffer();
		OVERLAPPED* Overlapped = new AsyncReadResult(_Handler);
		DWORD ReceivedBytes;
		DWORD Flags = 0;

		INT ret = WSARecv(_Handle, 
			SocketBuffers, 
			1,
			&ReceivedBytes, 
			&Flags, 
			Overlapped, 
			NULL);

			if(SOCKET_ERROR == ret)
		{
			INT ErrCode = WSAGetLastError();
			if(ErrCode != WSA_IO_PENDING && ErrCode != WSAEWOULDBLOCK)
			{
				//delete Overlapped;???
				return FALSE;
			}
		}

		return TRUE;
	}
	
	BOOL AsyncStream::AsyncWrite()
	{
		WSABUF*	SocketBuffer = _PacketBuffer->GetSockSendBuffer();
		DWORD	SentBytes;		
		OVERLAPPED* Overlapped = new AsyncWriteResult(_Handler);

		INT ret = WSASend(_Handle, 
			SocketBuffer, 
			_PacketBuffer->GetBufferSize(),
			&SentBytes, 
			0, 
			Overlapped, 
			NULL);

		if (SOCKET_ERROR == ret) 
		{ 
			INT ErrCode = WSAGetLastError();
			if (ErrCode != WSA_IO_PENDING) 
			{
				if (ErrCode == WSAECONNRESET || ErrCode == WSAEINVAL)
				{
				}
				else
				{
				}
				//delete Overlapped;//???
				return FALSE;
			} 
		}
		return TRUE;
	}


	BOOL SocketSubsystem::InitializeSubSystem()
	{
		// MSDN ���̺귯���� ������ �����ߴ�.
		WORD wVersionRequested;
		WSADATA wsaData;
		int err;

		wVersionRequested = MAKEWORD( 2, 2 );

		err = WSAStartup( wVersionRequested, &wsaData );
		if ( err != 0 ) {
			// Tell the user that we could not find a usable Winsock DLL.
			// ���� �ý��� �ʱ�ȭ�� ���������� WSAGetLastError �� �������� �ʴ´�.
			// \todo ���� ó��
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
				// \todo ���� �޽����� �ִ� ASSERT, VERIFY�� ������ ���ڴ�.
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