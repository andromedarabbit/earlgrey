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

		// TODO: 네트워크가 차단되었을 때 entry->HostName() 의 값이 255.255.255.255 로 나오는데 이를 명시적으로 오류 처리하던가 해야겠다.
		TEST_F(DnsTest, GetHostEntryWithIPAddress)
		{
			const _txstring hostNameOrAddress = _T("69.147.114.224");
			Dns::IPHostEntryPtr entry = Dns::GetHostEntry(hostNameOrAddress);
			ASSERT_EQ(1, entry->AddressList().size());
			ASSERT_CONTAINS(entry->HostName(), _T("yahoo"));		
		}

		TEST_F(DnsTest, GetHostName)
		{
			const _txstring hostName = Dns::GetHostName();
			ASSERT_FALSE(hostName.empty());
		}
	}
}