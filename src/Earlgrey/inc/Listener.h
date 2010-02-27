#pragma once

#include "Acceptor.h"

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
		AcceptCompleteHandler(SOCKET ListenSocket)
		{
			_ListenSocket.Attach( ListenSocket );
		}

		DWORD Run();

	private:
		Socket _ListenSocket;
	};

	class Listener : public IWaitHandler 
	{
	public:
		Listener(void);
		~Listener(void);

		void DoTask();

		BOOL Listen(USHORT Port, bool ReuseAddress);

	private:
		Socket _Socket;
		HANDLE _AcceptEvent;
	};

	class Server : public Listener
	{
	};

}
