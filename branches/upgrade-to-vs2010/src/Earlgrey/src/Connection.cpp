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
		_Stream = std::tr1::shared_ptr<AsyncStream>( new AsyncStream() );
		_Sender = std::tr1::shared_ptr<Sender>( new Sender( _Stream, NetEvent ) );
		_Receiver = std::tr1::shared_ptr<Receiver>( new Receiver( _Stream, NetEvent, PacketHandler ) );

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