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
		_Stream = make_ptr( new AsyncStream() );
		_Sender = make_ptr( new Sender( _Stream, NetEvent ) );
		_Receiver = make_ptr( new Receiver( _Stream, NetEvent, PacketHandler ) );

		_Stream->Initialize( socket, _Receiver.get(), _Sender.get(), &ProactorSingleton::Instance() );
		_Stream->Read();

		return true;
	}

	void Connection::SetTag( __int64 tag )
	{
		EARLGREY_ASSERT(_Stream.get());
		if (_Stream.get())
		{
			_Stream->SetTag( tag );
		}
	}

}