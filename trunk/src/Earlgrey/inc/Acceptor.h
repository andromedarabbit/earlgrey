#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"
#include "mswsock.h"
#include "NetworkBuffer.h"
#include "ThreadRunnable.h"
#include "WaitEvent.h"


namespace Earlgrey
{

	//! Accept �̺�Ʈ�� ����ϴ� �������� runnable Ŭ����
	/*!
		Listen �Ǵ� Connect�� ���� �� �ϴ��� Accept �̺�Ʈ�� �� �����忡���� ��ٸ���.
		
	*/
	class AcceptorRunnable : public ThreadRunnable
	{
	public:
		virtual void Stop() 
		{
			::InterlockedExchange( &_IsRunning, FALSE );
		}

		virtual void Exit() 
		{
		}

		BOOL MeetsStopCondition() const
		{
			return _IsRunning;
		}

		DWORD DoTask() 
		{
			WaitEventContainerSingleton::Instance().WaitEvents();
			return 0;
		}

	private:
		volatile LONG _IsRunning;
	};

	//class Acceptor : public CompletionHandler
	//{
	//public:
	//	BOOL SetProactor(Proactor* InProactor);

	//	// CompletionHandler Interface
	//	virtual void HandleEvent(AsyncResult* Result);

	//	BOOL Listen(USHORT Port, bool ReuseAddress = true);
	//	SOCKET CreateAcceptSocket(AsyncStream* InStream);

	//private:
	//	Socket _ListenSocket;		
	//	Socket _AcceptSocket;
	//	Proactor* _Proactor;
	//};
}