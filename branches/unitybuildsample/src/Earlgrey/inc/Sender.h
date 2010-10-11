#pragma once 

#include "CompletionHandler.h"

namespace Earlgrey
{
	class AsyncStream;

	class Sender : public CompletionHandler
	{
	public:
		explicit Sender(AsyncStream* Stream);

		virtual ~Sender();

		virtual void HandleEvent(AsyncResult* Result);

	private:
		AsyncStream* _Stream;
	};
}