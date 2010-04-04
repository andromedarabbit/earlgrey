#pragma once
#include "Socket.h"
#include "Acceptor.h"
#include "EarlgreyAssert.h"

namespace Earlgrey {

	//! Accept 이벤트가 왔을 때 관련 처리를 한다.
	/*!
		! IOCP 스레드에서 수행된다.
		1. 클라이언트 소켓을 생성하고 IOCP 에 등록한다.
		2. 패킷 수신을 시작한다.
	*/
	class AcceptCompleteHandler : public IRunnable
	{
	public:
		explicit AcceptCompleteHandler(SOCKET ListenSocket)
			: _ListenSocket( ListenSocket )
		{
			EARLGREY_ASSERT(ListenSocket != INVALID_SOCKET);
		}

		virtual DWORD Run();

	private:
		Socket _ListenSocket;
	};

	class Listener : public IWaitHandler 
	{
	public:
		explicit Listener(void);
		virtual ~Listener(void);

		// BOOL Listen(USHORT Port);
		BOOL Listen(const IPEndPoint& localEP);

		inline
			bool ExclusiveAddressUse() const 
		{
			return _ExclusiveAddressUse ;
		}

		void ExclusiveAddressUse(bool exclusiveAddressUse);

	protected:
		virtual void DoTask();

	private:
		Socket _Socket;
		HANDLE _AcceptEvent;
		bool m_Active;
		bool _ExclusiveAddressUse;
	};

	class Server : public Listener
	{
	};

}
