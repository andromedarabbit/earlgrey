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
			explicit MailAddress(const TCHAR * const address);
			explicit MailAddress(const TCHAR * const address, const TCHAR * displayName);

			~MailAddress();

			const _txstring& Address() const; 
			const _txstring& DisplayName() const;

		private:
			_txstring m_Address;
			_txstring m_DisplayName;
		};

		typedef std::tr1::shared_ptr<MailAddress> MailAddressPtr;


		inline MailAddress::MailAddress(const TCHAR * const address)
			: m_Address(address)
			, m_DisplayName(address)
		{

		}

		inline MailAddress::MailAddress(const TCHAR * const address, const TCHAR * displayName)
			: m_Address(address)
			, m_DisplayName(displayName)
		{

		}

		inline MailAddress::~MailAddress()
		{

		}

		inline const _txstring& MailAddress::Address() const 
		{
			return m_Address;
		}

		inline const _txstring& MailAddress::DisplayName() const 
		{
			return m_DisplayName;
		}

	} // !Mail
} // !Earlgrey