#pragma once 
#include "AsyncStream.h"

class ServerConnection : public Earlgrey::AsyncStream
{
public:
	ServerConnection () 
		: Earlgrey::AsyncStream()
	{}
	virtual ~ServerConnection () {}

	virtual void Connected()
	{
		printf("Server Socket = %d\r\n", Socket());
	}

	virtual void Disconnected()
	{
		printf("Server Disconnected %d", Socket());
	}

	virtual void Received()
	{

	}

	virtual void Sent()
	{

	}
};
