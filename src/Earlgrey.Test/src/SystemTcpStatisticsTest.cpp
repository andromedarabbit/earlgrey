#include "stdafx.h"
#include "SystemTcpStatistics.h"


namespace Earlgrey
{
// 	namespace Test
// 	{
		TEST(SystemTcpStatisticsTest, CreateInstance)
		{
			SystemTcpStatistics statistics(AF_INET);
			ASSERT_GT(statistics.ConnectionsAccepted(), 0);
			ASSERT_GT(statistics.ConnectionsInitiated(), 0);
			ASSERT_GT(statistics.CumulativeConnections(), 0);
			ASSERT_GT(statistics.CurrentConnections(), 0);
			ASSERT_GE(statistics.ErrorsReceived(), 0);
			ASSERT_GE(statistics.FailedConnectionAttempts(), 0);
			ASSERT_GT(statistics.MaximumConnections(), 0);
			ASSERT_GT(statistics.MaximumTransmissionTimeout(), 0);
			ASSERT_GT(statistics.MinimumTransmissionTimeout(), 0);
			ASSERT_GT(statistics.ResetConnections(), 0);
			ASSERT_GT(statistics.ResetsSent(), 0);
			ASSERT_GT(statistics.SegmentsReceived(), 0);
			ASSERT_GT(statistics.SegmentsResent(), 0);
			ASSERT_GT(statistics.SegmentsSent(), 0);
		}
//	}
}