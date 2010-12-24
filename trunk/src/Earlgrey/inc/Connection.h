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
		std::auto_ptr<AsyncStream> _Stream;		//!< IOCP ����� ó�� ��ü
		std::auto_ptr<Receiver> _Receiver;		//!< ��Ŷ ���� �Ϸ� ó����
		std::auto_ptr<Sender> _Sender;			//!< ��Ŷ ���� �Ϸ� ó����
	};

	
}