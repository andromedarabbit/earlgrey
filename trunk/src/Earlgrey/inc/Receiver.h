#pragma once 

#include "CompletionHandler.h"

namespace Earlgrey
{
	class AsyncStream;
	class AsyncResult;
	class NetworkBuffer;
	class IPacketHandler;
	class INetEvent;

	class Receiver : public CompletionHandler
	{
	public:
		explicit Receiver(
			AsyncStream* Stream, 
			std::tr1::shared_ptr<INetEvent> NetEvent, 
			std::tr1::shared_ptr<IPacketHandler> PacketHandler);

		virtual ~Receiver();

		// CompletionHandler Interface
		void HandleEvent(AsyncResult* Result);
	protected:
		AsyncStream* _Stream;
		std::tr1::shared_ptr<INetEvent> _NetEvent;
		std::tr1::shared_ptr<IPacketHandler> _PacketHandler;
		size_t _Start;	//!< ���ۿ��� ó���� �������� ���� ��ġ
		size_t _End;	//!< ���ۿ� ��ϵ� �������� ���� ����Ų��.
	};
}