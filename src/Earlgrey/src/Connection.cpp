#include "StdAfx.h"
#include "Connection.h"

namespace Earlgrey {

	Connection::Connection(void) 
	{
		_Stream.reset( new AsyncStream() );
		_Sender.reset( new Sender( _Stream.get() ) );
		_Receiver.reset( new Receiver( _Stream.get() ) );
	}

	Connection::~Connection(void)
	{
	}

	bool Connection::Initialize( const Socket& socket )
	{
		return Initialize(socket.GetHandle());
	}

	bool Connection::Initialize( SOCKET socket )
	{
		_Stream->Initialize( socket, _Receiver.get(), _Sender.get(), &ProactorSingleton::Instance() );
		_Stream->Read();

		return true;
	}

}