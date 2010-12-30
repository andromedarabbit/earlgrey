#pragma once

#include "INetEvent.h"

class NetServerEvent : public Earlgrey::INetEvent
{
public:
	NetServerEvent(void);
	~NetServerEvent(void);

public:
	//! 연결이 성공했을 때 호출된다.
	virtual void OnConnected(std::tr1::shared_ptr<Earlgrey::Connection> connection) 
	{
		UNREFERENCED_PARAMETER( connection );
	}

	//! 연결이 끊겼을 때 호출된다.
	virtual void OnDisconnected() 
	{

	}
};
