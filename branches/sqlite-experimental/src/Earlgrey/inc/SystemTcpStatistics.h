#pragma once
#include "Uncopyable.h"

#include <gtest/gtest_prod.h>
#include <Iphlpapi.h>

namespace Earlgrey
{
	class SystemTcpStatistics : private Uncopyable
	{
	private:
		explicit SystemTcpStatistics();

		FRIEND_TEST(SystemTcpStatisticsTest, CreateInstance);
		explicit SystemTcpStatistics(DWORD dwFamily);

	public:

		INT64 ConnectionsAccepted () const
		{
			return m_TCPStats.dwPassiveOpens;
		}

		INT64 ConnectionsInitiated() const
		{
			return m_TCPStats.dwActiveOpens;
		}

		INT64 CumulativeConnections() const
		{
			return m_TCPStats.dwNumConns;
		}

		INT64 CurrentConnections() const
		{
			return m_TCPStats.dwCurrEstab;
		}

		INT64 ErrorsReceived () const
		{
			return m_TCPStats.dwInErrs;
		}

		INT64 FailedConnectionAttempts() const
		{
			return m_TCPStats.dwAttemptFails;
		}

		INT64 MaximumConnections() const
		{
			return m_TCPStats.dwMaxConn;
		}

		INT64 MaximumTransmissionTimeout() const
		{
			return m_TCPStats.dwRtoMax;
		}

		INT64 MinimumTransmissionTimeout() const
		{
			return m_TCPStats.dwRtoMin;
		}

		INT64 ResetConnections() const
		{
			return m_TCPStats.dwEstabResets;
		}

		INT64 ResetsSent() const
		{
			return m_TCPStats.dwOutRsts;
		}

		INT64 SegmentsReceived() const
		{
			return m_TCPStats.dwInSegs; 
		}

		INT64 SegmentsResent () const
		{
			return m_TCPStats.dwOutSegs;
		}

		INT64 SegmentsSent () const
		{
			return m_TCPStats.dwRetransSegs;
		}
		
	private:
		MIB_TCPSTATS m_TCPStats;


	};
}