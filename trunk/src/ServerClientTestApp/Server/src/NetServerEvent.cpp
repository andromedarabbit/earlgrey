#include "StdAfx.h"
#include "..\inc\NetServerEvent.h"
#include "ConnectionContainer.h"

NetServerEvent::NetServerEvent(void)
{
}

NetServerEvent::~NetServerEvent(void)
{
}

void NetServerEvent::OnConnected( std::tr1::shared_ptr<Earlgrey::Connection> connection )
{
	ConnectionsSingleton::Instance().Add( connection );
}
