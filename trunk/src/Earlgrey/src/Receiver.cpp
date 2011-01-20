#include "stdafx.h"
#include "Receiver.h"
#include "AsyncStream.h"
#include "NetworkBuffer.h"
#include "INetEvent.h"
#include "IPacketHandler.h"

namespace Earlgrey
{
	Receiver::Receiver(
		std::tr1::shared_ptr<AsyncStream> Stream, 
		std::tr1::shared_ptr<INetEvent> NetEvent, 
		std::tr1::shared_ptr<IPacketHandler> PacketHandler) 
		: _Stream(Stream), _Start(0), _End(0), _NetEvent(NetEvent), _PacketHandler(PacketHandler)
	{
		EARLGREY_ASSERT( _NetEvent.get() );
		EARLGREY_ASSERT( _PacketHandler.get() );
		EARLGREY_ASSERT( _Stream.get() );
	}

	Receiver::~Receiver()
	{
	}

	void Receiver::HandleEvent(AsyncResult* Result)
	{
		EARLGREY_ASSERT( _NetEvent.get() );
		EARLGREY_ASSERT( _PacketHandler.get() );
		EARLGREY_ASSERT( _Stream.get() );

		if (Result->GetBytesTransferred() == 0)
		{
			_NetEvent->OnDisconnected( _Stream->GetTag() );
			return;
		}

		size_t oldEnd = _End;

		NetworkBuffer* buffer = _Stream->GetReadBuffer();
		DWORD readBytes = Result->GetBytesTransferred();
		buffer->OnReceived( readBytes );
		_End += readBytes;

		EARLGREY_ASSERT( _End - oldEnd < 100 );

		size_t HandledSize = 0;

		if (!_PacketHandler->Handle( _Stream, buffer, _Start, _End, HandledSize ))
		{
			_Stream->Close();
			_NetEvent->OnDisconnected( _Stream->GetTag() );
			return;
		}

		EARLGREY_ASSERT( _End - _Start >= HandledSize );
		if (_End - _Start < HandledSize)
		{
			_Stream->Close();
			_NetEvent->OnDisconnected( _Stream->GetTag() );
			return;
		}

		_Start += HandledSize;

		if (!_Stream->Read())
		{
			// Read() 에서 이미 Close() 가 호출된 상태임
			_NetEvent->OnDisconnected( _Stream->GetTag() );
		}
	}

}