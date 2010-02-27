#pragma once 
#include "Uncopyable.h"
#include "ServerInit.h"

namespace Earlgrey
{
	class IPAddress
	{
		SOCKADDR_IN Address;
	public:
		IPAddress()
		{
			memset(&Address,0, sizeof(SOCKADDR_IN));
			Address.sin_family = AF_INET;
		}

		void SetPort(INT Port)
		{
			Address.sin_port = htons((u_short)Port);
		}

		INT GetPort() const
		{
			return ntohs(Address.sin_port);
		}

		void SetAddr(const in_addr& Addr)
		{
			Address.sin_addr = Addr;
		}

		operator SOCKADDR_IN*()
		{
			return (SOCKADDR_IN*)&Address;
		}
	};

	class Dns : private Uncopyable
	{
	private:
		explicit Dns();

	public:
		static DWORD GetHostByName(LPCSTR Name, IPAddress& Addr)
		{
			HOSTENT* HostEnt = gethostbyname(Name);

			if (!HostEnt)
			{
				return WSAGetLastError();
			}

			if (HostEnt->h_addrtype == PF_INET)
			{
				Addr.SetAddr( *(in_addr*)(*HostEnt->h_addr_list) );
				return 0;
			}
			
			return WSAHOST_NOT_FOUND;
		}
	};

	
}