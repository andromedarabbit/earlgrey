#pragma once 

#include "Proactor.h"
#include "Socket.h"

namespace Earlgrey
{
	class Acceptor 
		: public CompletionHandler
	{
	public:
		explicit Acceptor(DWORD InPort)
		{
			Port = InPort;
			Initialize();
		};

		virtual ~Acceptor() {};

		BOOL Initialize();
		
		virtual void EventHandler(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* InOverlapped);
		virtual HANDLE GetHandle() { return (HANDLE)AcceptorSocket; };

	private:
		SOCKET AcceptorSocket;
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

		virtual void  EventHandler(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* InOverlapped);
		virtual HANDLE GetHandle() { return (HANDLE)ClientSocket; };

	public:
		SOCKET ClientSocket;
		AsyncStream Stream;

	};
}