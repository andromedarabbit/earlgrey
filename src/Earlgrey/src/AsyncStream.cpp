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
			if (_Handle != INVALID_SOCKET) // this line is not needed
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
			}
			return FALSE;
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
			} 
			return FALSE;
		}
		return TRUE;
	}
}