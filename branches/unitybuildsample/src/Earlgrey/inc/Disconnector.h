#pragma once 
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	class Disconnector
		: public CompletionHandler
	{
		explicit Disconnector()
		{

		};

		virtual ~Disconnector()
		{

		}

		BOOL Initialize();
		
		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes);
		virtual void HandleEventError(AsyncResult* Result, DWORD Error);
	};
}