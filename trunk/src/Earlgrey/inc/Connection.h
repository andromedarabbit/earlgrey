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
	class Connection
	{
	public:
		Connection(void);
		virtual ~Connection(void);

		// TODO: �ñ������� ���� �ڵ��� ���� �޴� �޼���� ���ִ� �� ����.
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

		NetworkBuffer* GetWriteBuffer()
		{
			return _Stream->GetWriteBuffer();
		}

	private:
		std::tr1::shared_ptr<AsyncStream> _Stream;		//!< IOCP ����� ó�� ��ü
		std::tr1::shared_ptr<Receiver> _Receiver;		//!< ��Ŷ ���� �Ϸ� ó����
		std::tr1::shared_ptr<Sender> _Sender;			//!< ��Ŷ ���� �Ϸ� ó����
	};

	
}