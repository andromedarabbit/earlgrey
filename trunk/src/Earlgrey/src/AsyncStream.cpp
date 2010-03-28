#include "stdafx.h"
#include "AsyncStream.h"

#include "Socket.h"
#include "Proactor.h"
#include "NetworkBuffer.h"

namespace Earlgrey
{
	//AsyncStream::AsyncStream()
	//{
	//	_PacketBuffer = new NetworkBuffer();
	//	_PacketBuffer->Initialize();
	//}

	//BOOL AsyncStream::Accept(USHORT Port)
	//{
	//	_Acceptor = new Acceptor(Port);//??? construct time and delete time
	//	if(!_Acceptor->CreateListenSocket())
	//		return FALSE;

	//	if((_Handle = _Acceptor->CreateAcceptSocket(this)) == INVALID_SOCKET)
	//		return FALSE;

	//	/*if(!_Acceptor->Register())
	//		return FALSE;*/

	//	return TRUE;
	//}

	//BOOL AsyncStream::Connect(const char* RemoteHostName, const INT Port)
	//{
	//	_Connector = new Connector();

	//	if((_Handle = _Connector->CreateSocket(RemoteHostName, Port, this)) == INVALID_SOCKET)
	//		return FALSE;

	//	if(!_Connector->Register())
	//		return FALSE;

	//	return TRUE;
	//}

	//BOOL AsyncStream::Disconnect()
	//{
	//	SOCKET OldSocket = (SOCKET)AtomicExch(_Handle, INVALID_SOCKET);

	//	LINGER Linger;
	//	Linger.l_onoff = 1;
	//	Linger.l_linger = 0;
	//	setsockopt(OldSocket, SOL_SOCKET, SO_LINGER, (const char*)&Linger, sizeof(Linger));
	//	closesocket(OldSocket);

	//	delete _PacketBuffer;//TODO

	//	return TRUE;
	//}

	//BOOL AsyncStream::Recv()
	//{
	//	WSABUF* SocketBuffers = _PacketBuffer->GetSockRecvBuffer();
	//	OVERLAPPED* Overlapped = new AsyncResult(_Receiver, this);
	//	DWORD ReceivedBytes;
	//	DWORD Flags = 0;

	//	INT ret = WSARecv(_Handle, 
	//		SocketBuffers, 
	//		1,
	//		&ReceivedBytes, 
	//		&Flags, 
	//		Overlapped, 
	//		NULL);

	//	if(SOCKET_ERROR == ret)
	//	{
	//		INT ErrCode = WSAGetLastError();
	//		if(ErrCode != WSA_IO_PENDING && ErrCode != WSAEWOULDBLOCK)
	//		{
	//			//delete Overlapped;???				
	//		}
	//		return FALSE;
	//	}

	//	return TRUE;
	//}

	//BOOL AsyncStream::Send()
	//{
	//	// TODO ; scatter&gatter mode 필요
	//	WSABUF*	SocketBuffer = _PacketBuffer->GetSockSendBuffer();
	//	DWORD	SentBytes;		
	//	OVERLAPPED* Overlapped = new AsyncResult(_Sender, this);

	//	INT ret = WSASend(_Handle, 
	//		SocketBuffer, 
	//		_PacketBuffer->GetBufferSize(),
	//		&SentBytes, 
	//		0, 
	//		Overlapped, 
	//		NULL);

	//	if (SOCKET_ERROR == ret) 
	//	{ 
	//		INT ErrCode = WSAGetLastError();
	//		if (ErrCode != WSA_IO_PENDING) 
	//		{
	//			if (ErrCode == WSAECONNRESET || ErrCode == WSAEINVAL)
	//			{
	//			}
	//			else
	//			{
	//			}
	//			//delete Overlapped;//???				
	//		} 
	//		return FALSE;
	//	}
	//	return TRUE;
	//}

	//BOOL AsyncStream::Post(CompletionHandler* Handler)
	//{
	//	return ProactorSingleton::Instance().PostEvent(new AsyncResult(Handler, this));
	//}



	void AsyncStream::Initialize(SOCKET handle, CompletionHandler* readHandler, CompletionHandler* writeHandler, Proactor* proactor)
	{
		_resultForRead.reset( new AsyncResult( handle, readHandler ) );
		_resultForWrite.reset( new AsyncResult( handle, writeHandler ) );
		_bufferForRead.reset( new NetworkBuffer() );
		_bufferForWrite.reset( new NetworkBuffer() );

		_handle = handle;
		_proactor = proactor;

		_proactor->RegisterHandler( (HANDLE)_handle, this );
	}

	bool AsyncStream::Read()
	{
		WSABUF* SocketBuffers = _bufferForRead->GetSockRecvBuffer();	// 크기가 NETWORK_BUFFER_DEFAULT_SIZE인 버퍼
		DWORD ReceivedBytes;
		DWORD Flags = 0;

		INT ret = WSARecv(_handle, 
			SocketBuffers, 
			1,
			&ReceivedBytes, 
			&Flags, 
			_resultForRead->GetOverlapped(), 
			NULL);

		if(SOCKET_ERROR == ret)
		{
			INT ErrCode = WSAGetLastError();
			if(ErrCode != WSA_IO_PENDING && ErrCode != WSAEWOULDBLOCK)
			{
				if (WSAECONNRESET == ErrCode)
				{
					// normal error
				}
				else
				{
					// warning
				}
				Close();
				return false;
			}			
		}

		return true;
	}

	bool AsyncStream::Write()
	{
		WSABUF*	SocketBuffer = _bufferForWrite->GetSockSendBuffer();
		DWORD	SentBytes;		

		INT ret = WSASend(_handle, 
			SocketBuffer, 
			_bufferForWrite->GetBufferSize(),
			&SentBytes, 
			0, 
			_resultForWrite->GetOverlapped(), 
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
				return false;
			} 
		}
		return true;
	}

	void AsyncStream::HandleEvent( AsyncResult* Result )
	{
		Result->HandleEvent();
	}

	NetworkBuffer* AsyncStream::GetReadBuffer()
	{
		return _bufferForRead.get();
	}

	NetworkBuffer* AsyncStream::GetWriteBuffer()
	{
		return _bufferForWrite.get();
	}
}