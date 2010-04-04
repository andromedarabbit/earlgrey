#pragma once
#include "Socket.h"
#include "Acceptor.h"
#include "EarlgreyAssert.h"

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
