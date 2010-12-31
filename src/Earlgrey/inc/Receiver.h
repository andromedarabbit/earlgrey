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
			std::tr1::shared_ptr<AsyncStream> Stream, 
			std::tr1::shared_ptr<INetEvent> NetEvent, 
			std::tr1::shared_ptr<IPacketHandler> PacketHandler);

		virtual ~Receiver();

		// CompletionHandler Interface
		void HandleEvent(AsyncResult* Result);
	protected:
		std::tr1::shared_ptr<AsyncStream> _Stream;
		std::tr1::shared_ptr<INetEvent> _NetEvent;
		std::tr1::shared_ptr<IPacketHandler> _PacketHandler;
		size_t _Start;	//!< 버퍼에서 처리할 데이터의 시작 위치
		size_t _End;	//!< 버퍼에 기록된 데이터의 끝을 가리킨다.
	};
}