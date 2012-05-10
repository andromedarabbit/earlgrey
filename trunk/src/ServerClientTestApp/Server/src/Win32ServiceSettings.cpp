#include "stdafx.h"
#include "Win32ServiceSettings.h"
#include "ServiceHelper.h"
#include "ExceptionEMailer.h"
#include "GMailClient.h"
#include "MiniDump.h"
#include "StackWriter.h"

using namespace Earlgrey;
using namespace Earlgrey::Mail;
using namespace Earlgrey::Extension;

const WCHAR * const Win32ServiceSettings::ShortName() const
{
	return L"EarlgreyServer";
}

//! \todo 코드 정리 좀....
std::tr1::shared_ptr<UnhandledExceptionHandler> Win32ServiceSettings::GetExceptionEMailer() const
{
	const xwstring miniDumpClassName(L"Earlgrey::MiniDump");
	const xwstring stackWriterClassName(L"Earlgrey::StackWriter");

	// 임시 코드
	const WCHAR * username = L"buildmaster@kaistizen.net";
	const WCHAR * password = L"dkswjsgkwldksgdmsdkagh";
	MailMessage::MailAddressPtr from(new MailAddress(L"buildmaster@kaistizen.net", L"빌드 마스터"));
	MailMessage::MailAddressPtr to1(new MailAddress(L"kaistizen@gmail.com", L"최재훈"));
	xwstring subject(L"[MiniDump] ");
	subject += this->ShortName();
	xwstring body(L"Please see attachments!");

	//! TODO: 바로 아래 코드에서 스레드가 둘 정도 더 생성된다.
	ExceptionEMailer::SmtpClientPtr smtpClient(new GMailClient(username, password));

	std::tr1::shared_ptr<ExceptionEMailer> sender( 
		new ExceptionEMailer(smtpClient) 
		);

	std::tr1::shared_ptr<MailMessage> message(new MailMessage(from, to1));
	message->Subject(subject.c_str());
	message->Body(body.c_str());

	AppSettings::UnhandledExceptionCollectionPtr collection = __super::UnhandledExceptions();
	AppSettings::UnhandledExceptionCollection::const_iterator it = collection->begin();
	for( ; it != collection->end(); it++)
	{
		std::tr1::shared_ptr<UnhandledExceptionHandler> handler( *it );
		if(
			handler->ClassName() != miniDumpClassName
			&& handler->ClassName() != stackWriterClassName
			)
		{
			continue;
		}

		if(handler->ClassName() == miniDumpClassName)
		{
			const MiniDump * miniDump = static_cast<MiniDump*>(handler.get());

			MailMessage::AttachmentPtr attachment(
				new Attachment(miniDump->FilePath().c_str(), L"")
				);
			message->Attachments().push_back(attachment);
		}

		if(handler->ClassName() == stackWriterClassName)
		{
			const StackWriter * stackWriter = static_cast<StackWriter*>(handler.get());

			MailMessage::AttachmentPtr attachment(
				new Attachment(stackWriter->FilePath().c_str(), L"")
				);
			message->Attachments().push_back(attachment);
		}
	}

	sender->AddMessage(message);
	return sender;
}

AppSettings::UnhandledExceptionCollectionPtr Win32ServiceSettings::UnhandledExceptions() const
{
	UnhandledExceptionCollectionPtr collection = __super::UnhandledExceptions();
	UnhandledExceptionCollectionPtr newCollection( new UnhandledExceptionCollection(*collection.get()));

	std::tr1::shared_ptr<UnhandledExceptionHandler> exceptionEMailer(GetExceptionEMailer());
	newCollection->push_back(exceptionEMailer);

	return newCollection;
}
