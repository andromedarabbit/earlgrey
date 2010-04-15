#pragma once
#include "Socket.h"
#include "NetworkStream.h"
#include "AsyncStream.h"
#include "Receiver.h"
#include "Sender.h"

namespace Earlgrey {

	class AsyncStream;
	class Receiver;
	class Sender;

	//! Remote Host���� ������ �����Ѵ�.
	template<template<class> class Reader, template<class> class Writer> 
	class Connection
	{
	public:
		typedef NetworkInputStream<Reader> InputStreamType;
		typedef NetworkOutputStream<Writer> OutputStreamType;

		Connection(void);
		virtual ~Connection(void);

		// TODO: �ñ������� ���� �ڵ��� ���� �޴� �޼���� ���ִ� �� ����.
		bool Initialize(const Socket& socket);
		bool Initialize(SOCKET socket);

		inline InputStreamType* GetInputStream();
		inline OutputStreamType* GetOutputStream();

	private:
		std::auto_ptr<AsyncStream> _Stream;		//!< IOCP ����� ó�� ��ü
		std::auto_ptr<Receiver> _Receiver;		//!< ��Ŷ ���� �Ϸ� ó����
		std::auto_ptr<Sender> _Sender;			//!< ��Ŷ ���� �Ϸ� ó����
		std::auto_ptr<InputStreamType> _InputStream;	//!< �Է� ��Ʈ�� ��ü; ��Ŷ �ڵ鷯���� ����Ѵ�.
		std::auto_ptr<OutputStreamType> _OutputStream;	//!< ��� ��Ʈ�� ��ü; ��Ŷ �ڵ鷯���� ����Ѵ�.
	};

	template<template<class> class Reader, template<class> class Writer> 
	inline Connection<Reader, Writer>::Connection(void) 
	{
		_InputStream.reset( new InputStreamType() );
		_OutputStream.reset( new OutputStreamType( _Stream.get() ) );
		_Stream.reset( new AsyncStream() );
		_Sender.reset( new Sender( _Stream.get() ) );
		_Receiver.reset( new Receiver( _Stream.get() ) );
	}

	template<template<class> class Reader, template<class> class Writer> 
	Connection<Reader, Writer>::~Connection(void)
	{
	}

	template<template<class> class Reader, template<class> class Writer> 
	bool Connection<Reader, Writer>::Initialize( const Socket& socket )
	{
		return Initialize(socket.GetHandle());
	}

	template<template<class> class Reader, template<class> class Writer> 
	bool Connection<Reader, Writer>::Initialize( SOCKET socket )
	{
		_Stream->Initialize( socket, _Receiver.get(), _Sender.get(), &ProactorSingleton::Instance() );
		_OutputStream->Initialize( _Stream->GetWriteBuffer() );
		_InputStream->Initialize( _Stream->GetReadBuffer() );

		_Stream->Read();

		return true;
	}

	template<template<class> class Reader, template<class> class Writer> 
	typename Connection<Reader, Writer>::InputStreamType* Connection<Reader, Writer>::GetInputStream()
	{
		return _InputStream;
	}

	template<template<class> class Reader, template<class> class Writer> 
	typename Connection<Reader, Writer>::OutputStreamType* Connection<Reader, Writer>::GetOutputStream()
	{
		return _OutputStream;
	}

	typedef Connection<DefaultReader, DefaultWriter> BinaryConnection;
}