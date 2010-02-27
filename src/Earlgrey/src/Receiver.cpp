#include "stdafx.h"
#include "Receiver.h"
#include "AsyncStream.h"


namespace Earlgrey
{
	Receiver::Receiver(AsyncStream* Stream) : _Stream(Stream)
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

		// TODO: 패킷을 모두 받은 후 핸들러를 호출한다.
	}

	bool Receiver::Read()
	{
		return _Stream->Read();
	}

	void Receiver::OnDisconnected()
	{

	}
}