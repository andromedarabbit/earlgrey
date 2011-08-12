#include "StdAfx.h"
#include "SocketHelper.h"
#include "Socket.h"
#include "WaitEvent.h"
#include "IPAddress.h"
#include "Dns.h"

namespace Earlgrey {


	void SocketHelper::SetAddress(IPAddress& Address, LPCSTR Server, int Port)
	{
		Dns::GetHostByName( Server, Address );
		Address.SetPort( Port );
	}

	bool SocketHelper::SetUpNonBlockingSocket(Socket& socket)
	{
		if (socket.IsValid())
		{
			return false;
		}

		if (!socket.CreateTcpSocket())
		{
			return false;
		}

		if (!socket.SetNonBlockingSocket())
		{
			socket.Close();
			return false;
		}

		return true;
	}

}