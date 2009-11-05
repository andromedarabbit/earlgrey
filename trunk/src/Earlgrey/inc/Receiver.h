#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"

namespace Earlgrey
{
	class Receiver
		: public CompletionHandler
	{
		explicit Receiver()
		{

		};

		virtual ~Receiver()
		{

		}

		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes)
		{
			//AsyncStream* Stream = Result->Stream();

			// passive �������
			if( TransferredBytes == 0 )
			{
				Result->Stream()->Disconnect();
			}
			else
			{
				//ConnectionHandler �� �Ѱ�����ϴµ�...
				//printf("���� �� = %s\n", Stream->GetNetworkBuffer().RecvBuf_);
				Result->Stream()->Recv();
			}
		}
		virtual void HandleEventError(AsyncResult* Result, DWORD Error);


	private:
	};
}