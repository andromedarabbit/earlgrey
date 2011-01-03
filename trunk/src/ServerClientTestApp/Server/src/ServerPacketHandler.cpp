#include "StdAfx.h"
#include "ServerPacketHandler.h"
#include "xvector.h"
#include "AsyncStream.h"
#include "NetworkBuffer.h"

ServerPacketHandler::ServerPacketHandler(void)
{
}

ServerPacketHandler::~ServerPacketHandler(void)
{
}

bool ServerPacketHandler::Handle( std::tr1::shared_ptr<Earlgrey::AsyncStream> Stream, Earlgrey::NetworkBuffer* Buffer, size_t Start, size_t End, size_t& HandledSize )
{
	UNREFERENCED_PARAMETER( Stream );
	UNREFERENCED_PARAMETER( Buffer );
	UNREFERENCED_PARAMETER( Start );
	UNREFERENCED_PARAMETER( End );
	UNREFERENCED_PARAMETER( HandledSize );

	typedef Earlgrey::xvector<BYTE>::Type ContainerType;

	ContainerType buf;
	buf.resize( End - Start );

	Stream->GetReadBuffer()->GetValue( Start, &buf[0], End - Start );

	ContainerType::const_iterator iter = buf.begin();
	for (; iter != buf.end(); iter++)
	{
		if (13 == *iter)
		{
			iter++;
			if (10 == *iter)
			{
				HandledSize = static_cast<size_t>( &(*iter) - &buf[0] + 1 );
				Stream->GetWriteBuffer()->SetValue( &buf[0], HandledSize );
				EARLGREY_VERIFY( Stream->Write() );
			}
		}
	}
	
	return true;
}
