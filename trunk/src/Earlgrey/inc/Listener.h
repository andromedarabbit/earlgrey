#pragma once

#include "Acceptor.h"

namespace Earlgrey {

	//! Accept �̺�Ʈ�� ���� �� ���� ó���� �Ѵ�.
	/*!
		! IOCP �����忡�� ����ȴ�.
		1. Ŭ���̾�Ʈ ������ �����ϰ� IOCP �� ����Ѵ�.
		2. ��Ŷ ������ �����Ѵ�.
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
