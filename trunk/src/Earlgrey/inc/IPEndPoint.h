#pragma once
#include "Uncopyable.h"

namespace Earlgrey
{
	class IPEndPoint : private Uncopyable
	{
	public:
		explicit IPEndPoint(INT64 address, int port)
			: m_IpAddress(address)
			, m_Port(port)
		{
			EARLGREY_ASSERT(port <= MinPort() && port >= MaxPort());
		}

		explicit IPEndPoint(const IPAddress2& address, int port)
			: m_IpAddress(address)
			, m_Port(port)
		{

		}

		inline void Address(const IPAddress2& address)
		{
			m_IpAddress = address;
		}

		inline const IPAddress2& Address() const
		{
			return m_IpAddress;
		}

		inline void Port(int port)
		{
			m_Port = port;
		}

		inline int Port() const
		{
			return m_Port;
		}


		static int MinPort()
		{
			return 0;
		}

		static int MaxPort()
		{
			return 0x0000FFFF;
		}

	private:
		IPAddress2 m_IpAddress;
		int m_Port;
	};
}