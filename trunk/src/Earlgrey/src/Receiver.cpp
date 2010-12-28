#include "stdafx.h"
#include "Receiver.h"
#include "AsyncStream.h"
#include "NetworkBuffer.h"

namespace Earlgrey
{
	Receiver::Receiver(AsyncStream* Stream) : _Stream(Stream), _Start(0), _End(0)
	{
	}

	Receiver::~Receiver()
	{
	}

	void Receiver::HandleEvent(AsyncResult* Result)
	{
		if (Result->GetBytesTransferred() == 0)
		{
			OnDisconnected();
			return;
		}

		NetworkBuffer* buffer = _Stream->GetReadBuffer();
		DWORD readBytes = Result->GetBytesTransferred();
		buffer->OnReceived( readBytes );

		if (!HandlePacket( buffer ))
		{
			_Stream->Close();
			return;
		}

		_Stream->Read();
	}

	void Receiver::OnDisconnected()
	{
		// TODO: Connection 객체를 제거한다.
	}

	bool Receiver::HandlePacket( NetworkBuffer* buffer )
	{
		UNREFERENCED_PARAMETER( buffer );
		return true;
	}

}