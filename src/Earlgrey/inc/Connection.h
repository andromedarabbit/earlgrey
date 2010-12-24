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
	class Connection
	{
	public:
		Connection(void);
		virtual ~Connection(void);

		// TODO: 궁극적으론 소켓 핸들을 직접 받는 메서드는 없애는 게 좋다.
		bool Initialize(const Socket& socket);
		bool Initialize(SOCKET socket);

		NetworkBuffer* GetReadBuffer()
		{
			return _Stream->GetReadBuffer();
		}

		NetworkBuffer* GetWriteBuffer()
		{
			return _Stream->GetWriteBuffer();
		}

	private:
		std::auto_ptr<AsyncStream> _Stream;		//!< IOCP 입출력 처리 객체
		std::auto_ptr<Receiver> _Receiver;		//!< 패킷 수신 완료 처리자
		std::auto_ptr<Sender> _Sender;			//!< 패킷 전송 완료 처리자
	};

	
}