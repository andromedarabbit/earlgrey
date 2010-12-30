#include "StdAfx.h"
#include "ServerPacketHandler.h"

ServerPacketHandler::ServerPacketHandler(void)
{
}

ServerPacketHandler::~ServerPacketHandler(void)
{
}

bool ServerPacketHandler::Handle( Earlgrey::NetworkBuffer* Buffer, size_t Start, size_t End, size_t& HandledSize )
{
	UNREFERENCED_PARAMETER( Buffer );
	UNREFERENCED_PARAMETER( Start );
	UNREFERENCED_PARAMETER( End );
	UNREFERENCED_PARAMETER( HandledSize );

	return true;
}
