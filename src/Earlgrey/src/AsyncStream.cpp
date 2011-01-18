#include "stdafx.h"
#include "AsyncStream.h"
#include "NetworkBuffer.h"
#include "Socket.h"
#include "Proactor.h"
#include <sstream>


namespace Earlgrey
{

	void AsyncStream::Initialize(SOCKET handle, CompletionHandler* readHandler, CompletionHandler* writeHandler, Proactor* proactor)
	{
		_resultForRead.reset( new AsyncResult( handle, readHandler ) );
		_bufferForRead.reset( new NetworkBuffer() );
		_writeHandler = writeHandler;

		_handle = handle;
		_proactor = proactor;

		_proactor->RegisterHandler( (HANDLE)_handle, this );
	}

	bool AsyncStream::Read()
	{
		EARLGREY_ASSERT(_bufferForRead.get());
		if (!_bufferForRead.get())
		{
			return false;
		}

		std::pair<WSABUF*,DWORD> SocketBuffers = _bufferForRead->GetSockRecvBuffer();	// 크기가 NETWORK_BUFFER_DEFAULT_SIZE인 버퍼
		DWORD Flags = 0;

		INT ret = WSARecv(_handle, 
			SocketBuffers.first, 
			SocketBuffers.second,
			NULL, 
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
					// forcibly closed by remote host; remote host uses hard(abortive) close
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

	void AsyncStream::_Write(std::tr1::shared_ptr<NetworkBuffer> buffer)
	{
		EARLGREY_ASSERT(buffer.get());
		if (!buffer.get())
		{
			return;
		}
		_ResultList.push_back( new AsyncResult( _handle, _writeHandler, buffer ) );

		return _WriteRemainder();
	}

	void AsyncStream::HandleEvent( AsyncResult* Result )
	{
		Result->HandleEvent();
	}

	NetworkBuffer* AsyncStream::GetReadBuffer()
	{
		return _bufferForRead.get();
	}

	void AsyncStream::Close()
	{
		LINGER l = {0};
		l.l_onoff = 1;
		l.l_linger = 0;

		setsockopt( _handle, SOL_SOCKET, SO_LINGER, (CHAR*) &l, sizeof(l) );
		closesocket( _handle );
	}

	AsyncStream::AsyncStream() : _handle(NULL), _proactor(NULL), _Sending(false)
	{
	}

	void AsyncStream::_WriteRemainder()
	{
		if (_Sending)
		{
			return;
		}
		if (_ResultList.empty())
		{
			return;
		}

		AsyncResult* result = _ResultList.front();
		EARLGREY_ASSERT(result);

		_Send( result );
	}

	bool AsyncStream::_Send(AsyncResult* Result)
	{
		DWORD SentBytes = 0;

		std::pair<WSABUF*,size_t> SocketBuffer = Result->GetWriteBuffer();

		INT ret = WSASend(_handle, 
			SocketBuffer.first, 
			EARLGREY_NUMERIC_CAST<DWORD>(SocketBuffer.second),
			&SentBytes, 
			0, 
			Result->GetOverlapped(), 
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
		_Sending = true;
		return true;
	}

	void AsyncStream::Write( std::tr1::shared_ptr<NetworkBuffer> buffer )
	{
		this->InvokeMethod( &AsyncStream::_Write, buffer );
	}

	void AsyncStream::OnSent( AsyncResult* Result )
	{
		this->InvokeMethod( &AsyncStream::_OnSent, Result );
	}

	void AsyncStream::_OnSent( AsyncResult* Result )
	{
		if (Result->SentCompleted())
		{
			EARLGREY_ASSERT( Result == _ResultList.front() );
			_ResultList.pop_front();
			delete Result;
		}
		_Sending = false;
		_WriteRemainder();
	}

}