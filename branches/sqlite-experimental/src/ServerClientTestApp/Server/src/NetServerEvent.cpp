#include "StdAfx.h"
#include "..\inc\NetServerEvent.h"
#include "ConnectionContainer.h"
#include "Connection.h"

NetServerEvent::NetServerEvent(void)
{
}

NetServerEvent::~NetServerEvent(void)
{
}

void NetServerEvent::OnConnected( std::tr1::shared_ptr<Earlgrey::Connection> connection )
{
	LONG connectionId = ConnectionsSingleton::Instance().Add( connection );
	connection->SetTag( connectionId );
}

void NetServerEvent::OnDisconnected(__int64 tag)
{
	ConnectionsSingleton::Instance().Remove( EARLGREY_NUMERIC_CAST<LONG>(tag) );
}
