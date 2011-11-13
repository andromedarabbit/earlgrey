#pragma once

namespace Earlgrey
{
	struct ProtocolType
	{
		// #define SOCK_STREAM     1               /* stream socket */
		// #define SOCK_DGRAM      2               /* datagram socket */
		// #define SOCK_RAW        3               /* raw-protocol interface */
		// #define SOCK_RDM        4               /* reliably-delivered message */
		// #define SOCK_SEQPACKET  5               /* sequenced packet stream */
		enum E
		{
			Unknown = -1
			, Unspecified = 0
			, Tcp = IPPROTO_TCP
		};
	};

}