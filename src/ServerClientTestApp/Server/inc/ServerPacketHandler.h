#pragma once

#include "IPacketHandler.h"

class ServerPacketHandler : public Earlgrey::IPacketHandler
{
public:
	ServerPacketHandler(void);
	~ServerPacketHandler(void);

	virtual bool Handle(Earlgrey::NetworkBuffer* Buffer, size_t Start, size_t End, size_t& HandledSize);
};
