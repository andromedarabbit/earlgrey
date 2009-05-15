#pragma once 
#include "Uncopyable.h"

namespace Earlgrey
{
	class Dns : private Uncopyable
	{
	private:
		explicit Dns(); // 구현하지 않음

	public:
		// static GetHostByName 
	};

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
}