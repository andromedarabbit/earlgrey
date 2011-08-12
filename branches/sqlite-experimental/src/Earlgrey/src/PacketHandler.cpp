#include "stdafx.h"

#include "PacketHandler.h"

namespace Earlgrey
{

	BOOL PacketHandler::HandlePacket(PacketIdType Id, ConnectionHandler* Connection, NetworkBuffer* Buffer, AsyncResult* Result )
	{
		Result;
		Connection;
		Buffer;

		if (_PacketHandlerTableSize >= Id)
		{
			return FALSE;
		}

		PacketHandlerFunction Function = _PacketHandlerTable[Id]._Handler;
		Function;

		return TRUE;
	}
}