#include "stdafx.h"
#include "Dns.h"

#include "IPHostEntry.h"
#include "IPAddress.h"
#include "RAII.h"
#include "StringHelper.h"
#include "numeric_cast.hpp"

#pragma warning( push ) 
#pragma warning( disable : 4996 )
#include <ws2tcpip.h>
#pragma warning( pop ) 

#pragma comment(lib, "Ws2_32.lib")

namespace Earlgrey
{
	_txstring Dns::GetHostName()
	{
		// TODO: 임시 버퍼
		static __declspec(thread) CHAR buffer[MAX_PATH];

		if(::gethostname(buffer, _countof(buffer)) != 0)
		{
			// WSAGetLastError
			throw std::exception("");
		}

#ifdef _UNICODE
		const int srcBytes = EARLGREY_NUMERIC_CAST<int>(strlen(buffer));
		const WCHAR * hostNameUnicode = String::ToUnicode(buffer, srcBytes);
		return _txstring(hostNameUnicode);
#else
		return _txstring(buffer);
#endif

	}

	DWORD Dns::GetHostByName(LPCSTR Name, IPAddress& Addr)
	{
		HOSTENT* HostEnt = gethostbyname(Name);

		if (!HostEnt)
		{
			return WSAGetLastError();
		}

		if (HostEnt->h_addrtype == PF_INET)
		{
			Addr.SetAddr( *(in_addr*)(*HostEnt->h_addr_list) );
			return 0;
		}

		return WSAHOST_NOT_FOUND;
	}

	void Dns::GetHostAddresses(const _txstring& hostNameOrAddress, IPAddresses& addresses)
	{	
		//--------------------------------
		// Setup the hints address info structure
		// which is passed to the getaddrinfo() function
		ADDRINFOT aiHints;

		memset(&aiHints, 0, sizeof(aiHints));
		// aiHints.ai_flags = AI_CANONNAME;
		// aiHints.ai_family = AF_UNSPEC;
		// 			aiHints.ai_socktype = SOCK_STREAM;
		// 			aiHints.ai_protocol = IPPROTO_TCP;

		ADDRINFOT * aiList = NULL;
		handle_t regKeyHandle(aiList, &FreeAddrInfo);

		const int retVal = GetAddrInfo(hostNameOrAddress.c_str(), NULL, &aiHints, &aiList);		
		if (retVal != 0) {
			throw std::exception("");
		}


		ADDRINFOT * current = aiList;
		do
		{
			IPAddressPtr ipAddress(
				new IPAddress2( 
				*reinterpret_cast<SOCKADDR_STORAGE*>(current->ai_addr) 
				)
				);
			addresses.push_back(ipAddress);
		} while ( NULL != (current = current->ai_next) );
	}

	namespace
	{
		_txstring TryGetNameInfo(const IPAddress2& addr, DWORD & errorCode)
		{
			_txstring firstIpAddress(addr.ToString());
#ifdef _UNICODE
			const int addressSize = EARLGREY_NUMERIC_CAST<int>(firstIpAddress.length() * sizeof(TCHAR));
			const CHAR * addressAnsi = String::FromUnicode(firstIpAddress.c_str(), addressSize);
#else
			const CHAR * addressAnsi = firstIpAddress.c_str();
#endif

			// struct sockaddr_in saGNI;
			SOCKADDR_IN saGNI;
			TCHAR hostname[NI_MAXHOST];
			TCHAR servInfo[NI_MAXSERV];

			// Set up sockaddr_in structure which is passed
			// to the getnameinfo function
			saGNI.sin_family = AF_INET;
			saGNI.sin_addr.s_addr = inet_addr(addressAnsi);
			// saGNI.sin_port = htons(80);


			//-----------------------------------------
			// Call getnameinfo
			errorCode = GetNameInfo(
				(struct sockaddr *) &saGNI,
				sizeof (struct sockaddr),
				hostname,
				NI_MAXHOST, servInfo, 
				NI_MAXSERV, NI_NUMERICSERV
				);

			return hostname;
		}

		Dns::IPHostEntryPtr InternalGetAddrinfo(const _txstring& hostNameOrAddress)
		{
			if(hostNameOrAddress.empty())
				throw std::exception("");

			//--------------------------------
			// Setup the hints address info structure
			// which is passed to the getaddrinfo() function
			ADDRINFOT aiHints;

			memset(&aiHints, 0, sizeof(aiHints));
			aiHints.ai_flags = AI_CANONNAME;
			aiHints.ai_family = AF_UNSPEC;
			// 			aiHints.ai_socktype = SOCK_STREAM;
			// 			aiHints.ai_protocol = IPPROTO_TCP;

			ADDRINFOT * aiList = NULL;
			handle_t regKeyHandle(aiList, &FreeAddrInfo);

			const int retVal = GetAddrInfo(hostNameOrAddress.c_str(), NULL, &aiHints, &aiList);		
			if (retVal != 0) {
				throw std::exception("");
			}


			Dns::IPHostEntryPtr entry( new IPHostEntry() );
			IPHostEntry::IPAddressContainer& entryAddresses = entry->AddressList();
			_txstring& hostName = entry->HostName();

			ADDRINFOT * current = aiList;
			do
			{
				Dns::IPAddressPtr ipAddress(
					new IPAddress2( 
					*reinterpret_cast<SOCKADDR_STORAGE*>(current->ai_addr) 
					)
					);
				entryAddresses.push_back(ipAddress);

				if(hostName.empty() && current->ai_canonname[0] != NULL)
					hostName = current->ai_canonname;

			} while ( NULL != (current = current->ai_next) );

			return entry;
		}

		Dns::IPHostEntryPtr InternalGetHostByAddress(Dns::IPAddressPtr address)
		{
			DWORD errCode = 0;
			_txstring name( TryGetNameInfo(*address, errCode) );
			if (errCode == 0)
			{
				return InternalGetAddrinfo(name);
			}

			Dns::IPHostEntryPtr entry(new IPHostEntry());
			entry->HostName() = address->ToString();
			entry->AddressList().push_back(address);
			return entry;
		}
	}


	Dns::IPHostEntryPtr Dns::GetHostEntry(const IPAddress2& address)
	{
		return GetHostEntry( address.ToString() );
	}

	Dns::IPHostEntryPtr Dns::GetHostEntry(const _txstring& hostNameOrAddress)
	{ 	
		if(hostNameOrAddress.empty())
			throw std::exception("");

		IPAddressPtr ipAddress( new IPAddress2(0) );
		if(IPAddress2::TryParse(hostNameOrAddress, *ipAddress) == TRUE)
		{
			if(*ipAddress == IPAddress2::Any())
				throw std::exception("net_invalid_ip_addr");
			return InternalGetHostByAddress(ipAddress);
		}

		// return InternalGetHostByName(hostNameOrAddress);
		return InternalGetAddrinfo(hostNameOrAddress);
	}

}