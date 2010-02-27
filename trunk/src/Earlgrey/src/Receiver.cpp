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

		// TODO: ��Ŷ�� ��� ���� �� �ڵ鷯�� ȣ���Ѵ�.
	}

	bool Receiver::Read()
	{
		return _Stream->Read();
	}

	void Receiver::OnDisconnected()
	{

	}
}