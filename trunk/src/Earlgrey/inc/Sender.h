#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"

namespace Earlgrey
{
	class Sender
		: public CompletionHandler
	{
		explicit Sender()
		{

		};

		virtual ~Sender()
		{

		}

		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes)
		{
			//AsyncStream* Stream = Result->Stream();

			// passive ¿¬°á²÷±è
			if( TransferredBytes == 0 )
			{
				Result->Stream()->Disconnect();
			}
			else
			{
			}
		}

		virtual void HandleEventError(AsyncResult* Result, DWORD Error);


	private:
	};
}