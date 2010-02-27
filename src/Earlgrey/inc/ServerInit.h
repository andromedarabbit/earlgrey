#pragma once 

#include "Thread.h"

#include "Socket.h"
#include "Dns.h"
#include "Proactor.h"
#include "Listener.h"
#include "Acceptor.h"
#include "Connector.h"
#include "Receiver.h"
#include "Sender.h"
#include "Disconnector.h"
#include "IOCP.h"
#include "AsyncStream.h"
#include "NetworkBuffer.h"
#include "PacketHandler.h"

namespace Earlgrey
{

	//void Init();
	void ServerCreate(DWORD Port);
	void ClientCreate(DWORD Port);
	void ServerCreated();

}
