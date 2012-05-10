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
			explicit MailAddress(const WCHAR * const address);
			explicit MailAddress(const WCHAR * const address, const WCHAR * displayName);

			~MailAddress();

			const xwstring& Address() const; 
			const xwstring& DisplayName() const;

		private:
			xwstring m_Address;
			xwstring m_DisplayName;
		};

		typedef std::tr1::shared_ptr<MailAddress> MailAddressPtr;


		inline MailAddress::MailAddress(const WCHAR * const address)
			: m_Address(address)
			, m_DisplayName(address)
		{

		}

		inline MailAddress::MailAddress(const WCHAR * const address, const WCHAR * displayName)
			: m_Address(address)
			, m_DisplayName(displayName)
		{

		}

		inline MailAddress::~MailAddress()
		{

		}

		inline const xwstring& MailAddress::Address() const 
		{
			return m_Address;
		}

		inline const xwstring& MailAddress::DisplayName() const 
		{
			return m_DisplayName;
		}

	} // !Mail
} // !Earlgrey