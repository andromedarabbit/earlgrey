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

	//! Remote Host와의 연결을 관리한다.
	template<template<class> class Reader, template<class> class Writer> 
	class Connection
	{
	public:
		typedef NetworkInputStream<Reader> InputStreamType;
		typedef NetworkOutputStream<Writer> OutputStreamType;

		Connection(void);
		virtual ~Connection(void);

		// TODO: 궁극적으론 소켓 핸들을 직접 받는 메서드는 없애는 게 좋다.
		bool Initialize(const Socket& socket);
		bool Initialize(SOCKET socket);

		inline InputStreamType* GetInputStream();
		inline OutputStreamType* GetOutputStream();

	private:
		std::auto_ptr<AsyncStream> _Stream;		//!< IOCP 입출력 처리 객체
		std::auto_ptr<Receiver> _Receiver;		//!< 패킷 수신 완료 처리자
		std::auto_ptr<Sender> _Sender;			//!< 패킷 전송 완료 처리자
		std::auto_ptr<InputStreamType> _InputStream;	//!< 입력 스트림 객체; 패킷 핸들러에서 사용한다.
		std::auto_ptr<OutputStreamType> _OutputStream;	//!< 출력 스트림 객체; 패킷 핸들러에서 사용한다.
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