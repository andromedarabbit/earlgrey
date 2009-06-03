#pragma once 

#include "Thread.h"

#include "Socket.h"
#include "Dns.h"
#include "Proactor.h"
#include "Acceptor.h"
#include "Connector.h"
#include "IOCP.h"
#include "NetworkBuffer.h"
#include "PacketHandler.h"

namespace Earlgrey
{

	void Init();
	void ServerCreate(DWORD Port);
	void ClientCreate(DWORD Port);
	void ServerCreated();

}
