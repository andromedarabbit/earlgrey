#pragma once 
#include "Acceptor.h"

class ServerConnection : public Earlgrey::ConnectionHandler
{
public:
	ServerConnection (HANDLE InHandle) 
		: Earlgrey::ConnectionHandler(InHandle)
	{}
	virtual ~ServerConnection () {}

	virtual void Connected()
	{
		printf("Server Socket = %d\r\n", Socket());
		Stream().AsyncRead();
	}

	virtual void Disconnected()
	{
		printf("Server Disconnected %d", Socket());
	}
};
