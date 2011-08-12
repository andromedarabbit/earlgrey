#pragma once 
#include "EarlgreyAssert.h"
#include "Socket.h"
#include "AsyncStream.h"
#include "IPAddress.h"
#include "WaitEvent.h"
#include "SocketHelper.h"

namespace Earlgrey
{
	class IPAddress;
	class INetEvent;
	class IPacketHandler;

	class Connector : public IWaitHandler
	{
	public:
		explicit Connector() : 
			_RetryCount(0), 
			_RetryInterval(0), 
			_Socket(INVALID_SOCKET),
			_WaitEvent(INVALID_HANDLE_VALUE),
			_Port(0)
		{
		}

		virtual ~Connector() 
		{
		}

		void Initialize(std::tr1::shared_ptr<INetEvent> NetEvent, std::tr1::shared_ptr<IPacketHandler> PacketHandler, BYTE RetryCount = 3, DWORD RetryInterval = 3000);

		bool ReConnect();

		bool Connect(LPCSTR Server, INT Port);

		//! ���� �̺�Ʈ�� ���� ����Ǵ� �޼����̴�.
		/*!
			������ �����ϸ� ��õ��Ѵ�. �����ϸ� Read �۾��� �����Ѵ�.
		*/
		bool DoTask();

	private:
		Socket _Socket;
		std::string _ServerName;
		INT _Port;
		HANDLE _WaitEvent;
		IPAddress _ServerAddress;
		BYTE _RetryCount;
		DWORD _RetryInterval;
		std::tr1::shared_ptr<IPacketHandler> _PacketHandler;
		std::tr1::shared_ptr<INetEvent> _NetEvent;
	};

}