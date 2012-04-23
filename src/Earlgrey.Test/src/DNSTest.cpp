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
			const _txstring hostNameOrAddress = _T("115.68.24.61");

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
			ASSERT_STREQ(_T("yahoo-wildcard.a05.yahoodns.net"), entry->HostName().c_str());
		}

		TEST_F(DnsTest, GetHostName)
		{
			const _txstring hostName = Dns::GetHostName();
			ASSERT_FALSE(hostName.empty());
		}
	}
}