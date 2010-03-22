#include "stdafx.h"
#include "DNS.h"
#include "IPHostEntry.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(DnsTest, GetHostAddressesWithHostName)
		{
			const _txstring hostNameOrAddress = _T("kaistizen.net");

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

		TEST(DnsTest, GetHostAddressesWithYahoo)
		{
			const _txstring hostNameOrAddress = _T("yahoo.com");

			Dns::IPAddresses addresses;
			Dns::GetHostAddresses(hostNameOrAddress, addresses);
			ASSERT_EQ(8, addresses.size());
		}

		TEST(DnsTest, GetHostAddressesWithIPAddress)
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

		TEST(DnsTest, GetHostEntryWithHostName)
		{
 			const _txstring hostNameOrAddress = _T("yahoo.com");
 			Dns::IPHostEntryPtr entry = Dns::GetHostEntry(hostNameOrAddress);
 			ASSERT_EQ(8, entry->AddressList().size());
		}

		TEST(DnsTest, GetHostEntryWithIPAddress)
		{
			const _txstring hostNameOrAddress = _T("69.147.114.224");
			Dns::IPHostEntryPtr entry = Dns::GetHostEntry(hostNameOrAddress);
			ASSERT_EQ(1, entry->AddressList().size());
			ASSERT_TRUE(entry->HostName() == _T("b1.www.vip.re3.yahoo.com"));			
		}

		TEST(DnsTest, GetHostName)
		{
			const _txstring hostName = Dns::GetHostName();
			ASSERT_FALSE(hostName.empty());
		}
	}
}