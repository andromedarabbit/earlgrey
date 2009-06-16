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

			if(InOverlapped->TransferredBytes() == 0)
			{
				Disconnected();
				break;
			}

			//_PacketHandler->HandlePacket(this, Stream.GetNetworkBuffer(), InOverlapped); //TODO : 
			NetworkBuffer* buf = Stream.GetNetworkBuffer();
			
			printf("Recevied : (%d)bytes, %s\r\n", InOverlapped->TransferredBytes(), buf->ToString());
			break;
			}

		case WRITE_EVENT:
			//write done process
			NetworkBuffer* buf = Stream.GetNetworkBuffer();
			printf("Sent : (%d)bytes, %s\r\n", InOverlapped->TransferredBytes(), buf->ToString());
			break;
		}
	}


}