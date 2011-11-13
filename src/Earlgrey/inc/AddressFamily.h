#pragma once

namespace Earlgrey
{
	struct AddressFamily
	{
		/*
		#define AF_UNSPEC       0               // unspecified
		#define AF_UNIX         1               // local to host (pipes, portals)
		#define AF_INET         2               // internetwork: UDP, TCP, etc.
		#define AF_IMPLINK      3               // arpanet imp addresses
		#define AF_PUP          4               // pup protocols: e.g. BSP
		#define AF_CHAOS        5               // mit CHAOS protocols
		#define AF_NS           6               // XEROX NS protocols
		#define AF_IPX          AF_NS           // IPX protocols: IPX, SPX, etc.
		#define AF_ISO          7               // ISO protocols
		#define AF_OSI          AF_ISO          // OSI is ISO
		#define AF_ECMA         8               // european computer manufacturers
		#define AF_DATAKIT      9               // datakit protocols
		#define AF_CCITT        10              // CCITT protocols, X.25 etc
		#define AF_SNA          11              // IBM SNA
		#define AF_DECnet       12              // DECnet
		#define AF_DLI          13              // Direct data link interface
		#define AF_LAT          14              // LAT
		#define AF_HYLINK       15              // NSC Hyperchannel
		#define AF_APPLETALK    16              // AppleTalk
		#define AF_NETBIOS      17              // NetBios-style addresses
		#define AF_VOICEVIEW    18              // VoiceView
		#define AF_FIREFOX      19              // Protocols from Firefox
		#define AF_UNKNOWN1     20              // Somebody is using this!
		#define AF_BAN          21              // Banyan
		#define AF_ATM          22              // Native ATM Services
		#define AF_INET6        23              // Internetwork Version 6
		#define AF_CLUSTER      24              // Microsoft Wolfpack
		#define AF_12844        25              // IEEE 1284.4 WG AF
		#define AF_IRDA         26              // IrDA
		#define AF_NETDES       28              // Network Designers OSI & gateway
		*/
		enum E
		{
			Unspecified = 0
			, InterNetwork = AF_INET
			, InterNetworkV6 = AF_INET6
		};
	};

}