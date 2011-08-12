#pragma once
#include "Socket.h"
#include "Acceptor.h"
#include "EarlgreyAssert.h"

namespace Earlgrey {

	class INetEvent;
	class IPacketHandler;

	//! Accept 이벤트가 왔을 때 관련 처리를 한다.
	/*!
		! IOCP 스레드에서 수행된다.
		1. 클라이언트 소켓을 생성하고 IOCP 에 등록한다.
		2. 패킷 수신을 시작한다.
	*/
	class AcceptCompleteHandler : public IRunnable
	{
	public:
		explicit AcceptCompleteHandler(SOCKET ListenSocket, std::tr1::shared_ptr<INetEvent> NetEvent, std::tr1::shared_ptr<IPacketHandler> PacketHandler);

		virtual DWORD Run();

	private:
		Socket _ListenSocket;
		std::tr1::shared_ptr<INetEvent> _NetEvent;
		std::tr1::shared_ptr<IPacketHandler> _PacketHandler;
	};

	class Listener : public IWaitHandler 
	{
	public:
		explicit Listener(void);
		virtual ~Listener(void);

		void Initialize(std::tr1::shared_ptr<INetEvent> NetEvent, std::tr1::shared_ptr<IPacketHandler> PacketHandler);

		// BOOL Listen(USHORT Port);
		BOOL Listen(const IPEndPoint& localEP);

		inline
			bool ExclusiveAddressUse() const 
		{
			return _ExclusiveAddressUse ;
		}

		void ExclusiveAddressUse(bool exclusiveAddressUse);

	protected:
		virtual bool DoTask();

	private:
		Socket _Socket;
		HANDLE _AcceptEvent;
		bool m_Active;
		bool _ExclusiveAddressUse;
		std::tr1::shared_ptr<INetEvent> _NetEvent;
		std::tr1::shared_ptr<IPacketHandler> _PacketHandler;
	};

	class Server : public Listener
	{
	};

}
