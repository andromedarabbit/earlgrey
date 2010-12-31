#pragma once 

#include "CompletionHandler.h"

namespace Earlgrey
{
	class AsyncStream;
	class NetEvent;
	class INetEvent;

	class Sender : public CompletionHandler
	{
	public:
		explicit Sender(std::tr1::shared_ptr<AsyncStream> Stream, std::tr1::shared_ptr<INetEvent> NetEvent);

		virtual ~Sender();

		virtual void HandleEvent(AsyncResult* Result);

	private:
		std::tr1::shared_ptr<AsyncStream> _Stream;
		std::tr1::shared_ptr<INetEvent> _NetEvent;
	};
}