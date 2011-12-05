#include "stdafx.h"
#include "IPAddress.h"

#include "txsstream.h"

namespace Earlgrey
{
	IPAddress2::IPAddress2(const AddressBytes& address)
		// : m_AddressBytes()
		: m_AddressStorage()
		, m_AddressFamily(AddressFamily::InterNetwork)
	{
		EARLGREY_ASSERT(address.size() == 4); 

		// std::copy(address.begin(), address.end(), m_AddressBytes.begin());
		memset(&m_AddressStorage, 0, sizeof(SOCKADDR_STORAGE));

		m_AddressStorage.ss_family = EARLGREY_NUMERIC_CAST<short>(m_AddressFamily);
		m_AddressStorage.__ss_pad1[2] = address[0];
		m_AddressStorage.__ss_pad1[3] = address[1];
		m_AddressStorage.__ss_pad1[4] = address[2];
		m_AddressStorage.__ss_pad1[5] = address[3];

	}

	IPAddress2::IPAddress2(const INT64 address)
		: m_AddressFamily(AddressFamily::InterNetwork)
	{
		const WORD lowWord = LOWORD(address);
		const WORD highWord = HIWORD(address);

		m_AddressStorage.ss_family = EARLGREY_NUMERIC_CAST<short>(m_AddressFamily);
		//  			m_AddressBytes.push_back(LOBYTE(lowWord));
		//  			m_AddressBytes.push_back(HIBYTE(lowWord));
		//  			m_AddressBytes.push_back(LOBYTE(highWord));
		//  			m_AddressBytes.push_back(HIBYTE(highWord));
		m_AddressStorage.__ss_pad1[2] = LOBYTE(lowWord);
		m_AddressStorage.__ss_pad1[3] = HIBYTE(lowWord);
		m_AddressStorage.__ss_pad1[4] = LOBYTE(highWord);
		m_AddressStorage.__ss_pad1[5] = HIBYTE(highWord);

	}

	// TODO: IP version 6용
	// 		IPAddress2::IPAddress2(const AddressBytes& address, INT64 scopeid)
	// 		{
	// 		}

	IPAddress2::IPAddress2(const SOCKADDR_STORAGE& address)
		: m_AddressStorage(address)
	{

	}

	IPAddress2 IPAddress2::Parse(const _txstring& ipString)
	{
		if(ipString.empty())
			throw std::exception("");

		SOCKADDR_STORAGE saDestination;
		int addrlen = sizeof(SOCKADDR_STORAGE);

		Socket::InitializeSockets(); // WSAStringToAddress 호출 전에...
		const int retValue = WSAStringToAddress( 
			(LPTSTR)ipString.c_str(), // 강제 캐스팅해도 되나?
			// buffer,
			AF_INET,
			NULL,
			(SOCKADDR*)&saDestination,
			&addrlen
			);

		if (retValue == SOCKET_ERROR) {
			const DWORD errCode = WSAGetLastError();
			const CHAR * const errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}

		AddressBytes bytes;
		bytes.push_back(saDestination.__ss_pad1[2]);
		bytes.push_back(saDestination.__ss_pad1[3]);
		bytes.push_back(saDestination.__ss_pad1[4]);
		bytes.push_back(saDestination.__ss_pad1[5]);
		return IPAddress2(bytes);
	}

	BOOL IPAddress2::TryParse(const _txstring& ipString, IPAddress2& address)
	{
		try
		{
			address = Parse(ipString);
			return TRUE;
		}
		catch (std::exception& ex)
		{
			DBG_UNREFERENCED_LOCAL_VARIABLE(ex);
			return FALSE;
		}		
	}

	void IPAddress2::GetAddressBytes(IPAddress2::AddressBytes& addressBytes) const
	{
		// AddressBytes bytes;
		addressBytes.push_back(m_AddressStorage.__ss_pad1[2]);
		addressBytes.push_back(m_AddressStorage.__ss_pad1[3]);
		addressBytes.push_back(m_AddressStorage.__ss_pad1[4]);
		addressBytes.push_back(m_AddressStorage.__ss_pad1[5]);
		// return bytes;
	}

	_txstring IPAddress2::ToString() const
	{
		_txostringstream oss;
		oss << static_cast<BYTE>(m_AddressStorage.__ss_pad1[2]) << _T(".")
			<< static_cast<BYTE>(m_AddressStorage.__ss_pad1[3]) << _T(".")
			<< static_cast<BYTE>(m_AddressStorage.__ss_pad1[4]) << _T(".")
			<< static_cast<BYTE>(m_AddressStorage.__ss_pad1[5])
		;
		return oss.str();
	}

	IPAddress2 IPAddress2::Any()
	{
		return IPAddress2(INADDR_ANY);
	}

	IPAddress2 IPAddress2::Loopback()
	{
		return IPAddress2(INADDR_LOOPBACK);
	}

	IPAddress2 IPAddress2::Broadcast()
	{
		return IPAddress2(INADDR_BROADCAST);
	}

	IPAddress2 IPAddress2::None()
	{
		return IPAddress2(INADDR_NONE);
	}
}