#pragma once 
#include "EarlgreyAssert.h"
#include "AsyncStream.h"
// #include "IOCPEventType.h"
#include "Proactor.h"
#include "mswsock.h"
#include "NetworkBuffer.h"



namespace Earlgrey
{
	class Acceptor 
		: public CompletionHandler
	{
	public:
		explicit Acceptor(USHORT InPort)
			: ListenSocket(INVALID_SOCKET),
			AcceptSocket(INVALID_SOCKET),
			Port(InPort)
		{};

		virtual ~Acceptor() {};

		BOOL Register();		

		// CompletionHandler Interface
		virtual void HandleEvent(AsyncResult* Result, DWORD TransferredBytes);
		virtual void HandleEventError(AsyncResult* Result, DWORD Error);

		BOOL CreateListenSocket();
		SOCKET CreateAcceptSocket(AsyncStream* InStream);

	private:
		SOCKET ListenSocket;		
		SOCKET AcceptSocket;
		USHORT Port;
	};


	/**
	*/
	inline BOOL Acceptor::Register()
	{
		if (!ProactorSingleton::Instance().RegisterHandler( (HANDLE)AcceptSocket, this))
		{
			if (AcceptSocket != INVALID_SOCKET) // this line is not needed
			{
				closesocket(AcceptSocket);
			}
			return FALSE;
		}
		if (!ProactorSingleton::Instance().RegisterHandler( (HANDLE)ListenSocket, this))
		{
			if (ListenSocket != INVALID_SOCKET) // this line is not needed
			{
				closesocket(ListenSocket);
			}
			return FALSE;
		}
		return TRUE;
	}

	inline void Acceptor::HandleEventError(AsyncResult* /*Result*/, DWORD /*Error*/)
	{
		return;//TODO :
	}
}