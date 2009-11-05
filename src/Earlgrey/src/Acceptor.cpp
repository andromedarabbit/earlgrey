#include "stdafx.h"

#include "ServerInit.h"
#include "Acceptor.h"
#include "NetworkBuffer.h"

namespace Earlgrey
{

	/*BOOL ConnectionHandler::Initialize()
	{
		//stream_.Open(socket_, this);
		//Stream.AsyncRead();

		return TRUE;
	}*/

	//void ConnectionHandler::HandleEvent(AsyncResult* Result, DWORD TransferredBytes)
	//{
	//	//EARLGREY_ASSERT(Handle == (HANDLE)socket_);
	//	switch(Type)
	//	{
	//	case READ_EVENT:
	//		{

	//		if(TransferredBytes == 0)
	//		{
	//			Disconnected();
	//			break;
	//		}

	//		//_PacketHandler->HandlePacket(this, Stream.GetNetworkBuffer(), InOverlapped); //TODO : 
	//		NetworkBuffer* buf = stream_.GetNetworkBuffer();
	//		
	//		printf("Recevied : (%d)bytes, %s\r\n", TransferredBytes, buf->ToString());
	//		break;
	//		}

	//	case WRITE_EVENT:
	//		//write done process
	//		NetworkBuffer* buf = stream_.GetNetworkBuffer();
	//		printf("Sent : (%d)bytes, %s\r\n", TransferredBytes, buf->ToString());
	//		break;
	//	}
	//}
			
	//void ConnectionHandler::HandleEventError(AsyncResult* Result, DWORD Error)
	//{

	//}


}