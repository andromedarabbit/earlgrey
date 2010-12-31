#pragma once

#include "IPacketHandler.h"

class ServerPacketHandler : public Earlgrey::IPacketHandler
{
public:
	ServerPacketHandler(void);
	~ServerPacketHandler(void);

	virtual bool Handle(std::tr1::shared_ptr<Earlgrey::AsyncStream> Stream, Earlgrey::NetworkBuffer* Buffer, size_t Start, size_t End, size_t& HandledSize);
};
