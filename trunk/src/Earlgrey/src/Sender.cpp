#include "stdafx.h"
#include "Sender.h"
#include "NetworkBuffer.h"
#include "AsyncStream.h"
#include "INetEvent.h"

namespace Earlgrey
{
	Sender::Sender(std::tr1::shared_ptr<AsyncStream> Stream, std::tr1::shared_ptr<INetEvent> NetEvent) 
		: _Stream(Stream), _NetEvent(NetEvent)
	{
	}

	Sender::~Sender()
	{

	}

	void Sender::HandleEvent(AsyncResult* Result)
	{
		if (Result->GetBytesTransferred() == 0)
		{
			return;
		}
		if (Result->GetBytesTransferred() == _Stream->GetWriteBuffer()->GetBufferSize())
		{
			_Stream->GetWriteBuffer()->Clear();
		}
	}

}