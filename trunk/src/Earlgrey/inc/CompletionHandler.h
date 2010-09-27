#pragma once

namespace Earlgrey {

	class AsyncResult;

	class CompletionHandler : private Uncopyable
	{
	public:
		explicit CompletionHandler() {};
		virtual ~CompletionHandler() {};

		virtual void HandleEvent(AsyncResult* Result) = 0;
	};

}