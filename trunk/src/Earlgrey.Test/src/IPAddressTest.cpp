#include "stdafx.h"
#include "IPAddress.h"

#include "SocketTest.h"

namespace Earlgrey
{
	namespace Test
	{
		class IPAddressTest : public SocketTest {
		};


		TEST_F(IPAddressTest, ReadBoolean)
		{
			IN_ADDR addr;
			addr.S_un.S_un_b.s_b1 = 250;
			addr.S_un.S_un_b.s_b2 = 125;
			addr.S_un.S_un_b.s_b3 = 62;
			addr.S_un.S_un_b.s_b4 = 31;

			const IPAddress2 ipAddress(addr.S_un.S_addr);
			// const IPAddress2::AddressBytes& bytes = ipAddress.GetAddressBytes();
			IPAddress2::AddressBytes bytes;
			ipAddress.GetAddressBytes(bytes);

			ASSERT_EQ(bytes[0], addr.S_un.S_un_b.s_b1);
			ASSERT_EQ(bytes[1], addr.S_un.S_un_b.s_b2);
			ASSERT_EQ(bytes[2], addr.S_un.S_un_b.s_b3);
			ASSERT_EQ(bytes[3], addr.S_un.S_un_b.s_b4);
		}

		TEST_F(IPAddressTest, ParseIPAddressString)
		{
			// const _txstring ipString = _T("3ffe:2900:d005:f28d:250:8bff:fea0:92ed");
			const _txstring ipString = _T("115.68.22.160");
			IPAddress2 address = IPAddress2::Parse(ipString);

			IPAddress2::AddressBytes bytes;
			address.GetAddressBytes(bytes);

			ASSERT_EQ(bytes[0], 115);
			ASSERT_EQ(bytes[1], 68);
			ASSERT_EQ(bytes[2], 22);
			ASSERT_EQ(bytes[3], 160);
		}

		TEST_F(IPAddressTest, Parse2)
		{
			const _txstring ipString = _T("115.68.22.160");
			IPAddress2 address = IPAddress2::Parse(ipString);

			IPAddress2::AddressBytes bytes;
			address.GetAddressBytes(bytes);

			ASSERT_EQ(bytes[0], 115);
			ASSERT_EQ(bytes[1], 68);
			ASSERT_EQ(bytes[2], 22);
			ASSERT_EQ(bytes[3], 160);
		}

		TEST_F(IPAddressTest, TryParseIpAddress)
		{
			const _txstring ipString = _T("115.68.22.160");
			IPAddress2 address(0);
			ASSERT_TRUE(IPAddress2::TryParse(ipString, address));

			IPAddress2::AddressBytes bytes;
			address.GetAddressBytes(bytes);

			ASSERT_EQ(bytes[0], 115);
			ASSERT_EQ(bytes[1], 68);
			ASSERT_EQ(bytes[2], 22);
			ASSERT_EQ(bytes[3], 160);
		}

		TEST_F(IPAddressTest, TryParseNonIpAddress)
		{
			const _txstring ipString = _T("1154.68.22.160");
			IPAddress2 address(0);
			ASSERT_FALSE(IPAddress2::TryParse(ipString, address));
		}

		TEST_F(IPAddressTest, ToString)
		{
			const _txstring ipString = _T("115.68.22.160");
			IPAddress2 address = IPAddress2::Parse(ipString);

			const _txstring ipStringSecond = address.ToString();
			ASSERT_TRUE(ipString == ipStringSecond);
		}
	}
}