#include "stdafx.h"

#include "ServerInit.h"
#include "Acceptor.h"

namespace Earlgrey
{

	/**
	*/
	BOOL ConnectionHandler::Initialize()
	{
		Stream.Open(Socket, this);
		//Stream.AsyncRead();

		return TRUE;
	}

	void ConnectionHandler::HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* InOverlapped)
	{
		EARLGREY_ASSERT(Handle == (HANDLE)Socket);
		switch(Type)
		{
		case READ_EVENT:
			{

			//_PacketHandler->HandlePacket(this, Stream.GetNetworkBuffer(), InOverlapped);
			NetworkBuffer* buf = Stream.GetNetworkBuffer();
			
			printf("(%d) %s", InOverlapped->TransferredBytes(), buf->ToString());
			break;
			}

		case WRITE_EVENT:
			//write done process
			break;
		}
	}


}