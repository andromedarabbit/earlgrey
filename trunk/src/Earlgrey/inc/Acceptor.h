#pragma once 
#include "Thread.h"
#include "Proactor.h"
#include "Socket.h"

namespace Earlgrey
{
	class Acceptor 
		: public WaitEventHandler
	{
	public:
		explicit Acceptor(DWORD InPort)
			: AcceptorSocket(INVALID_SOCKET),
			Port(InPort)
		{};

		virtual ~Acceptor() {};

		BOOL Initialize();
		
		// CompletionHandler Interface
		virtual void HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* Result);
		virtual HANDLE GetHandle() { return (HANDLE)AcceptorSocket; };

		// WaitEventHandler Interface
		virtual void HandleEvent();

	private:
		SOCKET AcceptorSocket;
		WSAEVENT AcceptorEvent;
		AsyncStream Stream;

		DWORD Port;
	};

	class ConnectionHandler 
		: public CompletionHandler
	{
	public:
		explicit ConnectionHandler(HANDLE InHandle)
		{
			ClientSocket = (SOCKET)InHandle;
			Initialize();
		};

		BOOL Initialize();

		virtual ~ConnectionHandler() {};

		virtual void  HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* Result);
		virtual HANDLE GetHandle() { return (HANDLE)ClientSocket; };

	public:
		SOCKET ClientSocket;
		AsyncStream Stream;

	};

}