#pragma once
#include "Uncopyable.h"
#include "MailAddress.h"
#include "Attachment.h"

namespace Earlgrey
{
	namespace Mail
	{
		class MailMessage : private Uncopyable
		{
		public:
			typedef std::tr1::shared_ptr<MailAddress> MailAddressPtr;
			typedef xlist<MailAddressPtr>::Type MailAddressCollection;

			typedef std::tr1::shared_ptr<Attachment> AttachmentPtr;
			typedef xlist<AttachmentPtr>::Type AttachmentCollection;

			explicit MailMessage()
				: m_FromAddress()
				, m_ToAddresses()
				, m_Subject()
				, m_Body()
				, m_Attachments()
			{

			}

			explicit MailMessage(MailAddressPtr from, MailAddressPtr to)
				: m_FromAddress(from)
				, m_ToAddresses()
				, m_Subject()
				, m_Body()
				, m_Attachments()
			{
				m_ToAddresses.push_back(to);
			}

			explicit MailMessage(const TCHAR * from, const TCHAR * to)
				: m_FromAddress()
				, m_ToAddresses()
				, m_Subject()
				, m_Body()
				, m_Attachments()
			{
				m_FromAddress = MailAddressPtr(new MailAddress(from));

				MailAddressPtr toAddress(new MailAddress(to));
				m_ToAddresses.push_back(toAddress);
			}

			inline MailAddressCollection& To() 
			{
				return m_ToAddresses;
			}

			inline const MailAddressCollection& To() const 
			{
				return m_ToAddresses;
			}

			inline MailAddressPtr& From() 
			{
				return m_FromAddress;
			}

			inline const MailAddressPtr& From() const 
			{
				return m_FromAddress;
			}

			inline const _txstring& Subject() const 
			{
				return m_Subject;
			}

			inline void Subject(const TCHAR * const subject)
			{
				m_Subject = subject;
			}

			inline const _txstring& Body() const 
			{
				return m_Body;
			}

			inline void Body(const TCHAR * const body)
			{
				m_Body = body;
			}

			inline AttachmentCollection& Attachments() 
			{
				return m_Attachments;
			}

			inline const AttachmentCollection& Attachments() const 
			{
				return m_Attachments;
			}

		private:
			MailAddressPtr m_FromAddress;
			MailAddressCollection m_ToAddresses;			
			_txstring m_Subject;
			_txstring m_Body;
			AttachmentCollection m_Attachments;

		};
	} // !Mail
}