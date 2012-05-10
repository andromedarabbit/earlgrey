#pragma once
#include "UnhandledExceptionHandler.h"
#include "SmtpClient.h"
#include "xlist.h"

namespace Earlgrey
{
	// template <typename S>
	class ExceptionEMailer : public UnhandledExceptionHandler
	{
	public:
		// typedef S SmtpClientType;
		typedef Earlgrey::Mail::SmtpClient SmtpClientType;
		typedef std::tr1::shared_ptr<SmtpClientType> SmtpClientPtr;
		typedef std::tr1::shared_ptr<Earlgrey::Mail::MailMessage> MailMessagePtr;
		typedef xlist<MailMessagePtr>::Type MailMessageCollection;

		explicit ExceptionEMailer(
			SmtpClientPtr smtpClient
			// , const MailMessageCollection& messages
			);
		virtual ~ExceptionEMailer();

		virtual const WCHAR * ClassName() const;

		virtual void HandleException(LPEXCEPTION_POINTERS exceptionPtr);

		inline void AddMessage(MailMessagePtr message)
		{
			m_MailMessages.push_back(message);
		}

	private:
		SmtpClientPtr m_SmtpClient;
		MailMessageCollection m_MailMessages;
	};

	/*
	template <typename S>
	ExceptionEMailer<S>::ExceptionEMailer(
		SmtpClientPtr smtpClient
		, const MailMessageCollection& messages
		)
		: m_SmtpClient(smtpClient)
		, m_MailMessages(messages)
	{

	}

	template <typename S>
	ExceptionEMailer<S>::~ExceptionEMailer()
	{
	}

	template <typename S>
	void ExceptionEMailer<S>::HandleException(LPEXCEPTION_POINTERS exceptionPtr)
	{
		DBG_UNREFERENCED_PARAMETER(exceptionPtr);

		MailMessageCollection::const_iterator it = m_MailMessages.begin();
		for( ; it != m_MailMessages.end(); it++)
		{
			m_SmtpClient->Send(*it);
		}
	}
	*/
}