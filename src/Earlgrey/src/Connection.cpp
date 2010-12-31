#include "StdAfx.h"
#include "Connection.h"
#include "shared_ptr_helper.h"

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
		_Stream = make_ptr(new (alloc<AsyncStream>()) AsyncStream() );
		_Sender = make_ptr(new (alloc<Sender>()) Sender( _Stream, NetEvent ) );
		_Receiver = make_ptr(new (alloc<Receiver>()) Receiver( _Stream, NetEvent, PacketHandler ) );

		_Stream->Initialize( socket, _Receiver.get(), _Sender.get(), &ProactorSingleton::Instance() );
		_Stream->Read();

		return true;
	}

}