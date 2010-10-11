#include "stdafx.h"
#include "Sender.h"
#include "NetworkBuffer.h"
#include "AsyncStream.h"

namespace Earlgrey
{
	Sender::Sender(AsyncStream* Stream) : _Stream(Stream)
	{
	}

	Sender::~Sender()
	{

	}

	void Sender::HandleEvent(AsyncResult* Result)
	{
		if (Result->GetBytesTransferred() == 0)
		{
			//OnDisconnected();
			return;
		}
		if (Result->GetBytesTransferred() == _Stream->GetWriteBuffer()->GetBufferSize())
		{
			_Stream->GetWriteBuffer()->Clear();
		}
	}

}