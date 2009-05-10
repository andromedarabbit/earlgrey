#pragma once 

#include "Proactor.h"

namespace Earlgrey
{
	class Server : IOHandler
	{
	public:
		explicit Server(DWORD InPort)
			: Port(InPort)
		{	
		}

		~Server();

		BOOL Create(DWORD InPort);

		virtual void IODone(BOOL InSuccess, DWORD InTransferred, LPOVERLAPPED InOverlapped);

	private:
		SOCKET _ServerSocket;
		OVERLAPPED _OverlappedRead;
		DWORD Port;
	};
}