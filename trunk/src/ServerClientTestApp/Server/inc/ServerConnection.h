#pragma once 
#include "AsyncStream.h"
#include "tiostream.h"

class ServerConnection : public Earlgrey::AsyncStream
{
public:
	explicit ServerConnection () 
		: Earlgrey::AsyncStream()
	{}
	virtual ~ServerConnection () {}

	virtual void Connected()
	{
		Earlgrey::_tcout << _T("Server Socket = ") << Socket() << std::endl;
	}

	virtual void Disconnected()
	{
		Earlgrey::_tcout << _T("Server Disconnected ") << Socket() << std::endl;
	}

	virtual void Received()
	{

	}

	virtual void Sent()
	{

	}
};
