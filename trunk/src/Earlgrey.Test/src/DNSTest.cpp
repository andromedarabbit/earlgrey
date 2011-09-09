#include "stdafx.h"
#include "DNS.h"

#include "IPHostEntry.h"
#include "IPAddress.h"
#include "SocketTest.h"

namespace Earlgrey
{
	namespace Test
	{
		class DnsTest : public SocketTest {
		};

		TEST_F(DnsTest, GetHostAddressesWithHostName)
		{
			const _txstring hostNameOrAddress = _T("AndromedaRabbit.net");

			Dns::IPAddresses addresses;
			Dns::GetHostAddresses(hostNameOrAddress, addresses);

			Dns::IPAddressPtr ipAddress(addresses.front());
			IPAddress2::AddressBytes bytes;
			ipAddress->GetAddressBytes(bytes);

			ASSERT_EQ(bytes[0], 115);
			ASSERT_EQ(bytes[1], 68);
			ASSERT_EQ(bytes[2], 24);
			ASSERT_EQ(bytes[3], 61);
		}

		TEST_F(DnsTest, GetHostAddressesWithYahoo)
		{
			const _txstring hostNameOrAddress = _T("yahoo.com");

			Dns::IPAddresses addresses;
			Dns::GetHostAddresses(hostNameOrAddress, addresses);
			ASSERT_GT(addresses.size(), static_cast<size_t>(1));
		}

		TEST_F(DnsTest, GetHostAddressesWithIPAddress)
		{
			const _txstring hostNameOrAddress = _T("115.68.22.160");

			Dns::IPAddresses addresses;
			Dns::GetHostAddresses(hostNameOrAddress, addresses);

			Dns::IPAddressPtr ipAddress(addresses.front());
			IPAddress2::AddressBytes bytes;
			ipAddress->GetAddressBytes(bytes);

			ASSERT_EQ(bytes[0], 115);
			ASSERT_EQ(bytes[1], 68);
			ASSERT_EQ(bytes[2], 22);
			ASSERT_EQ(bytes[3], 160);
		}

		TEST_F(DnsTest, GetHostEntryWithHostName)
		{
 			const _txstring hostNameOrAddress = _T("yahoo.com");
 			Dns::IPHostEntryPtr entry = Dns::GetHostEntry(hostNameOrAddress);
 			ASSERT_GT(entry->AddressList().size(), static_cast<size_t>(1));
		}

		TEST_F(DnsTest, GetHostEntryWithIPAddress)
		{
			const _txstring hostNameOrAddress = _T("69.147.114.224");
			Dns::IPHostEntryPtr entry = Dns::GetHostEntry(hostNameOrAddress);
			ASSERT_EQ(1, entry->AddressList().size());
			ASSERT_TRUE(entry->HostName() == _T("b1.www.vip.re3.yahoo.com"));			
		}

		TEST_F(DnsTest, GetHostName)
		{
			const _txstring hostName = Dns::GetHostName();
			ASSERT_FALSE(hostName.empty());
		}
	}
}