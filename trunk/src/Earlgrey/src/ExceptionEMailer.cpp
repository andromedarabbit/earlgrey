#include "stdafx.h"
#include "ExceptionEMailer.h"


namespace Earlgrey
{
	ExceptionEMailer::ExceptionEMailer(
		SmtpClientPtr smtpClient
		// , const MailMessageCollection& messages
		)
		: m_SmtpClient(smtpClient)
		// , m_MailMessages(messages)
	{

	}

	ExceptionEMailer::~ExceptionEMailer()
	{
	}

	const WCHAR * ExceptionEMailer::ClassName() const
	{
		return L"Earlgrey::ExceptionEMailer";
	}


	void ExceptionEMailer::HandleException(LPEXCEPTION_POINTERS exceptionPtr)
	{
		DBG_UNREFERENCED_PARAMETER(exceptionPtr);

		MailMessageCollection::const_iterator it = m_MailMessages.begin();
		for( ; it != m_MailMessages.end(); it++)
		{
			MailMessagePtr message(*it);
			m_SmtpClient->Send(*message.get());
		}
	}

}