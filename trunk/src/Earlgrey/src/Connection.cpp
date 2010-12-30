#include "StdAfx.h"
#include "Connection.h"

namespace Earlgrey {

	Connection::Connection(void) 
	{
	}

	Connection::~Connection(void)
	{
	}

	bool Connection::Initialize(
		const Socket& socket, 
		std::tr1::shared_ptr<INetEvent> NetEvent, 
		std::tr1::shared_ptr<IPacketHandler> PacketHandler)
	{
		return Initialize(socket.GetHandle(), NetEvent, PacketHandler);
	}

	bool Connection::Initialize( 
		SOCKET socket, 
		std::tr1::shared_ptr<INetEvent> NetEvent, 
		std::tr1::shared_ptr<IPacketHandler> PacketHandler)
	{
		_Stream.reset( new AsyncStream() );
		_Sender.reset( new Sender( _Stream.get(), NetEvent ) );
		_Receiver.reset( new Receiver( _Stream.get(), NetEvent, PacketHandler ) );

		_Stream->Initialize( socket, _Receiver.get(), _Sender.get(), &ProactorSingleton::Instance() );
		_Stream->Read();

		return true;
	}

}