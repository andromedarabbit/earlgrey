#include "stdafx.h"
#include "AsyncStream.h"
#include "NetworkBuffer.h"
#include "Socket.h"
#include "Proactor.h"


namespace Earlgrey
{

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
			EARLGREY_NUMERIC_CAST<DWORD>(_bufferForWrite->GetBufferSize()),
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