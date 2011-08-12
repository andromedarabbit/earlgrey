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
		bool Initialize(const Socket& socket, 
			std::tr1::shared_ptr<INetEvent> NetEvent, 
			std::tr1::shared_ptr<IPacketHandler> PacketHandler);

		bool Initialize(SOCKET socket, 
			std::tr1::shared_ptr<INetEvent> NetEvent, 
			std::tr1::shared_ptr<IPacketHandler> PacketHandler);

		NetworkBuffer* GetReadBuffer()
		{
			return _Stream->GetReadBuffer();
		}

		std::tr1::shared_ptr<AsyncStream> GetStream()
		{
			return _Stream;
		}

		//! tag 값을 설정한다. 이 값은 INetEvent::OnDisconnecte() 메서드의 파라미터로 넘어간다.
		void SetTag(__int64 tag);

	private:
		std::tr1::shared_ptr<AsyncStream> _Stream;		//!< IOCP 입출력 처리 객체
		std::tr1::shared_ptr<Receiver> _Receiver;		//!< 패킷 수신 완료 처리자
		std::tr1::shared_ptr<Sender> _Sender;			//!< 패킷 전송 완료 처리자
	};

	
}