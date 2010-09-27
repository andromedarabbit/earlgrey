#pragma once 

#include "CompletionHandler.h"

namespace Earlgrey
{
	class AsyncStream;
	class AsyncResult;

	class Receiver : public CompletionHandler
	{
	public:
		explicit Receiver(AsyncStream* Stream);

		virtual ~Receiver();

		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result);

		void OnDisconnected();

	private:
		AsyncStream* _Stream;
	};
}