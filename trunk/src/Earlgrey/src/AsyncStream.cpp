#include "stdafx.h"

#include "ServerInit.h"
#include "Socket.h"

namespace Earlgrey
{
	AsyncStream::AsyncStream()
	{
		_PacketBuffer = new NetworkBuffer();
		_PacketBuffer->Initialize();
	}

	BOOL AsyncStream::Accept(USHORT Port)
	{
		_Acceptor = new Acceptor(Port);//??? construct time and delete time
		if(!_Acceptor->CreateListenSocket())
			return FALSE;

		if((_Handle = _Acceptor->CreateAcceptSocket(this)) == INVALID_SOCKET)
			return FALSE;

		/*if(!_Acceptor->Register())
			return FALSE;*/

		return TRUE;
	}

	BOOL AsyncStream::Connect(const char* RemoteHostName, const INT Port)
	{
		_Connector = new Connector();

		if((_Handle = _Connector->CreateSocket(RemoteHostName, Port, this)) == INVALID_SOCKET)
			return FALSE;

		if(!_Connector->Register())
			return FALSE;

		return TRUE;
	}

	BOOL AsyncStream::Disconnect()
	{
		SOCKET OldSocket = (SOCKET)AtomicExch(_Handle, INVALID_SOCKET);

		LINGER Linger;
		Linger.l_onoff = 1;
		Linger.l_linger = 0;
		setsockopt(OldSocket, SOL_SOCKET, SO_LINGER, (const char*)&Linger, sizeof(Linger));
		closesocket(OldSocket);

		delete _PacketBuffer;//TODO

		return TRUE;
	}

	BOOL AsyncStream::Recv()
	{
		WSABUF* SocketBuffers = _PacketBuffer->GetSockRecvBuffer();
		OVERLAPPED* Overlapped = new AsyncResult(_Receiver, this);
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

	BOOL AsyncStream::Send()
	{
		// TODO ; scatter&gatter mode ÇÊ¿ä
		WSABUF*	SocketBuffer = _PacketBuffer->GetSockSendBuffer();
		DWORD	SentBytes;		
		OVERLAPPED* Overlapped = new AsyncResult(_Sender, this);

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

	BOOL AsyncStream::Post(CompletionHandler* Handler)
	{
		return ProactorSingleton::Instance().PostEvent(new AsyncResult(Handler, this));
	}


}