#pragma once
#include "xvector.h"
// #include <array>
#include "Log.h"

namespace Earlgrey
{

	class IPAddress
	{
		SOCKADDR_IN Address;
	public:
		IPAddress()
		{
			memset(&Address,0, sizeof(SOCKADDR_IN));
			Address.sin_family = AF_INET;
		}

		void SetPort(INT Port)
		{
			Address.sin_port = htons((u_short)Port);
		}

		INT GetPort() const
		{
			return ntohs(Address.sin_port);
		}

		void SetAddr(const in_addr& Addr)
		{
			Address.sin_addr = Addr;
		}

		operator SOCKADDR_IN*()
		{
			return (SOCKADDR_IN*)&Address;
		}
	};

	// TODO: �̹� ���� Ŭ������ �־ �ӽ� �̸����� ...
	// ����� IP version 4�� ������
	class IPAddress2 // : private Uncopyable
	{
	public:
		typedef xvector<BYTE>::Type AddressBytes;
		// typedef std::tr1::array<BYTE, 32> AddressBytes;

		explicit IPAddress2(const AddressBytes& address);
		explicit IPAddress2(const INT64 address);

		// TODO: IP version 6��
// 		explicit IPAddress2(const AddressBytes& address, INT64 scopeid);
		explicit IPAddress2(const SOCKADDR_STORAGE& address);

		// const AddressBytes& GetAddressBytes() const 
		void GetAddressBytes(AddressBytes& addressBytes) const;

// 		static BOOL IsLoopback(const IPAddress2& address)
// 		{
// 
// 		}

		_txstring ToString() const;

		// IPv4�� ��� �� ���� �� �ڸ� ǥ������� ǥ���� IP �ּҸ� �����ϴ� ���ڿ��̰�,
		// IPv6�� ��� �ݷа� 16���� ǥ������� ǥ���� IP �ּҸ� �����ϴ� ���ڿ��Դϴ�. 
		static IPAddress2 Parse(const _txstring& ipString);
		static BOOL TryParse(const _txstring& ipString, IPAddress2& address);

		static IPAddress2 Any();
		static IPAddress2 Loopback();
		static IPAddress2 Broadcast();
		static IPAddress2 None();

		// TODO: IPv6�� ���� �������� ����
// 		static IPAddress2 IPv6Any()

		inline 
			bool operator == (const IPAddress2& obj) const
		{
			return memcmp(&(this->m_AddressStorage), &(obj.m_AddressStorage), sizeof(SOCKADDR_STORAGE)) == 0;
		}

		inline 
			bool operator != (const IPAddress2& obj) const
		{
			return !(operator == (obj));
		}

	private:
		// AddressBytes m_AddressBytes;
		SOCKADDR_STORAGE m_AddressStorage;
	};
}