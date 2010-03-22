#pragma once
#include "xlist.h"

namespace Earlgrey
{
	class IPAddress2;

	class IPHostEntry
	{
	public:
		typedef std::tr1::shared_ptr<IPAddress2> IPAddressPtr;
		typedef xlist<IPAddressPtr>::Type IPAddressContainer;

		typedef xlist<_txstring>::Type AliasContainer;

		explicit IPHostEntry();

		inline
			const IPAddressContainer& AddressList() const
		{
			return m_AddressList;
		}

		inline
			IPAddressContainer& AddressList()
		{
			return m_AddressList;
		}

		inline
			const AliasContainer& Aliases() const
		{
			return m_Aliases;
		}

		inline
			AliasContainer& Aliases()
		{
			return m_Aliases;
		}

		inline 
			const _txstring& HostName() const
		{
			return m_HostName;
		}

		inline 
			_txstring& HostName()
		{
			return m_HostName;
		}

// 		inline
// 			void HostName(const _txstring& hostName)
// 		{
// 			m_HostName = hostName;
// 		}

	private:
		IPAddressContainer m_AddressList;
		AliasContainer m_Aliases;
		_txstring m_HostName;
	};
}