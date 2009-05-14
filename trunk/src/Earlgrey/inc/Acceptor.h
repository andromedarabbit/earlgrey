#pragma once 
#include "Thread.h"
#include "Proactor.h"
#include "Socket.h"

namespace Earlgrey
{
	class Acceptor 
		: public CompletionHandler, WaitEventHandler
	{
	public:
		explicit Acceptor(DWORD InPort)
		{
			Port = InPort;
			Initialize();
		};

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

	class AcceptorRunnable : public IRunnable
	{
	public:
		explicit AcceptorRunnable() {}
		virtual ~AcceptorRunnable() {}

		virtual BOOL Init();
		virtual DWORD Run();
		virtual void Stop() {}
		virtual void Exit() {}

		void Init(WSAEVENT event, WaitEventHandler* handler);//! TODO

	private:
		WSAEVENT				Event;
		WaitEventHandler*		WaitHandler;
	};
}