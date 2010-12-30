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
		explicit Sender(AsyncStream* StreamNetEvent, std::tr1::shared_ptr<INetEvent> NetEvent);

		virtual ~Sender();

		virtual void HandleEvent(AsyncResult* Result);

	private:
		AsyncStream* _Stream;
		std::tr1::shared_ptr<INetEvent> _NetEvent;
	};
}