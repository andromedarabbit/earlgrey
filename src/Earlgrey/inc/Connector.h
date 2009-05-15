#pragma once 

#include "Socket.h"
#include "Proactor.h"
#include "Dns.h"

namespace Earlgrey
{
	class Connector
		: public CompletionHandler, WaitEventHandler
	{
	public:
		explicit Connector() {}
		virtual ~Connector() {}

		virtual void HandleEvent(HANDLE Handle, IOCP_EVENT_TYPE Type, AsyncResult* Result);
		virtual HANDLE GetHandle() {return (HANDLE)ConnectorSocket;};

		virtual void HandleEvent();

		void Initialize();

		BOOL Connect(const char* RemoteHostName, const INT Port);
		void Disconnect();
		void Close();

	private:
		SOCKET ConnectorSocket;
		IPAddress ServerAddress;
		WSAEVENT ConnectorEvent;
		AsyncStream Stream;
	};
}