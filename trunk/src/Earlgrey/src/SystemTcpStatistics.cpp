#include "stdafx.h"
#include "SystemTcpStatistics.h"

#pragma comment(lib, "Iphlpapi.lib")

namespace Earlgrey
{
	SystemTcpStatistics::SystemTcpStatistics(DWORD dwFamily)
	{
		EARLGREY_ASSERT(dwFamily == AF_INET || dwFamily == AF_INET6);

		if(GetTcpStatisticsEx(&m_TCPStats, dwFamily) != NO_ERROR)
			throw std::exception("");
	}
}