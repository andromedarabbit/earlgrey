#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"

namespace Earlgrey
{
	class AsyncReadStream;

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