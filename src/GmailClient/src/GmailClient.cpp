#include "GMailClient.h"

#include "CSmtp.h"

// Earlgrey
#include "StringHelper.h"
#include "txsstream.h"
#include "MailMessage.h"

namespace Earlgrey
{
	namespace Extension
	{

		GMailClient::GMailClient(const TCHAR * username, const TCHAR * password)
			: m_Host(_T("smtp.gmail.com"))
			, m_Port(587)
			, m_UserName(username)
			, m_Password(password)
		{

		}

		GMailClient::~GMailClient()
		{

		}

		void GMailClient::Send(const Earlgrey::Mail::MailMessage& msg)
		{
		
			try
			{
				CSmtp mail;

				mail.SetSMTPServer(
					String::FromUnicode(m_Host)
					, EARLGREY_NUMERIC_CAST<unsigned short>(m_Port)
					);
				mail.SetSecurityType(USE_TLS);


				mail.SetLogin(String::FromUnicode(m_UserName));
				mail.SetPassword(String::FromUnicode(m_Password));

				

				mail.SetSenderName(String::FromUnicode(msg.From()->DisplayName()));
				mail.SetSenderMail(String::FromUnicode(msg.From()->Address()));
				mail.SetReplyTo(String::FromUnicode(msg.From()->Address()));
				mail.SetSubject(String::FromUnicode(msg.Subject()));

				Mail::MailMessage::MailAddressCollection::const_iterator it;
				for(it = msg.To().begin(); it != msg.To().end(); it++)
				{
					Mail::MailMessage::MailAddressPtr to = *it;
					const xstring email = String::FromUnicode(to->Address());
					const xstring displayName = String::FromUnicode(to->DisplayName());
					mail.AddRecipient(email.c_str(), displayName.c_str());
				}
				
				mail.SetXPriority(XPRIORITY_NORMAL);
				mail.SetXMailer("EarlGrey Sample Smtp Client");
				
				
				_txistringstream ss(msg.Body());
				_txstring line;

				while(getline(ss, line))
				{
					mail.AddMsgLine(String::FromUnicode(line));
				}

				Mail::MailMessage::AttachmentCollection::const_iterator a_it;
				for(a_it = msg.Attachments().begin(); a_it != msg.Attachments().end(); a_it++)
				{
					Mail::MailMessage::AttachmentPtr attachment = *a_it;
					mail.AddAttachment(String::FromUnicode(attachment->FileName()));
				}
				
				mail.Send();
			}
			catch(const ECSmtp& e)
			{				
				const char * errorMsg = e.GetErrorText().c_str();
				throw std::exception(errorMsg);
			}
		}

		void GMailClient::Host(const TCHAR * host)
		{
			m_Host = host;
		}

		const TCHAR * GMailClient::Host()
		{
			return m_Host.c_str();
		}

		void GMailClient::Port(int port)
		{
			m_Port = port;
		}

		const int GMailClient::Port()
		{
			return m_Port;
		}

	} // end of namespace Extension
} // end of namespace Earlgrey