#pragma once

#include "INetEvent.h"

class NetServerEvent : public Earlgrey::INetEvent
{
public:
	NetServerEvent(void);
	~NetServerEvent(void);

public:
	//! ������ �������� �� ȣ��ȴ�.
	virtual void OnConnected(std::tr1::shared_ptr<Earlgrey::Connection> connection) 
	{
		UNREFERENCED_PARAMETER( connection );
	}

	//! ������ ������ �� ȣ��ȴ�.
	virtual void OnDisconnected() 
	{

	}
};
