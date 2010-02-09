#pragma once
#include "Uncopyable.h"
#include "txstring.h"
#include "xlist.h"

namespace Earlgrey
{
	namespace Mail
	{
		class MailAddress : private Uncopyable
		{
		public:
			explicit MailAddress(const TCHAR * const address)
				: m_Address(address)
				, m_DisplayName(address)
			{

			}

			explicit MailAddress(const TCHAR * const address, const TCHAR * displayName)
				: m_Address(address)
				, m_DisplayName(displayName)
			{

			}

			inline const _txstring& Address() const 
			{
				return m_Address;
			}

			inline const _txstring& DisplayName() const 
			{
				return m_DisplayName;
			}

		private:
			_txstring m_Address;
			_txstring m_DisplayName;
		};

		typedef std::tr1::shared_ptr<MailAddress> MailAddressPtr;

	} // !Mail


} // !Earlgrey