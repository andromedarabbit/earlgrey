#include "stdafx.h"
#define EXP_STL
#include "GmailClient.h"
#include "MailMessage.h"
#include "Environment.h"
#include "File.h"
#include "Path.h"
#include "ExceptionEMailer.h"
#include "DefaultAppSettings.h"

#include "MiniDump.h"
#include "StackWriter.h"


namespace Earlgrey
{
	namespace Extension
	{
		namespace Test
		{
			struct Anonymous
			{
				static xwstring CreateMockDumpFile(const xwstring& dstFile)
				{
					using namespace std;

					const xwstring baseDir = Environment::BaseDirectoryW();
					const xwstring srcFile( Path::Combine(baseDir, L"Earlgrey.Test.txt") );
					// xwstring dstFile( Path::Combine(baseDir, L"GMailClientTest.cpp") ); // __FILE__


					BOOL succeeded = File::Exists(srcFile);
					// ASSERT_TRUE2(succeeded);

					if( File::Exists(dstFile.c_str()) )
					{
						succeeded = File::Delete(dstFile);
						// ASSERT_TRUE2(succeeded);
					}

					// succeeded = ::CopyFile(srcFile.c_str(), dstFile.c_str(), true);
					succeeded = File::Copy(srcFile, dstFile, true);
					// ASSERT_TRUE2(succeeded);

					return dstFile;
				}

				static std::tr1::shared_ptr<ExceptionEMailer> GetExceptionEMailer()
				{
					using namespace Earlgrey::Mail;

					DefaultAppSettings appSettings;

					const xwstring miniDumpClassName(L"Earlgrey::MiniDump");
					const xwstring stackWriterClassName(L"Earlgrey::StackWriter");

					// 임시 코드
					const WCHAR * username = L"projectearlgrey@gmail.com";
					const WCHAR * password = L"dkswjsgkwldksgdmsdkagh";
					MailMessage::MailAddressPtr from(new MailAddress(L"projectearlgrey@gmail.com", L"빌드 마스터"));
					MailMessage::MailAddressPtr to1(new MailAddress(L"projectearlgrey@gmail.com", L"최재훈"));
					xwstring subject(L"[MiniDump] ");
					subject += appSettings.ShortName();
					xwstring body(L"Please see attachments!");

					ExceptionEMailer::SmtpClientPtr smtpClient(new GMailClient(username, password));

					std::tr1::shared_ptr<ExceptionEMailer> sender( 
						new ExceptionEMailer(smtpClient) 
						);

					std::tr1::shared_ptr<MailMessage> message(new MailMessage(from, to1));
					message->Subject(subject.c_str());
					message->Body(body.c_str());

					AppSettings::UnhandledExceptionCollectionPtr collection = appSettings.UnhandledExceptions();
					AppSettings::UnhandledExceptionCollection::const_iterator it = collection->begin();
					for( ; it != collection->end(); it++)
					{
						// ExceptionEMailer * emailer = static_cast<ExceptionEMailer*>(sender.get());

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

							CreateMockDumpFile(miniDump->FilePath());
						}

						if(handler->ClassName() == stackWriterClassName)
						{
							const StackWriter * stackWriter = static_cast<StackWriter*>(handler.get());

							MailMessage::AttachmentPtr attachment(
								new Attachment(stackWriter->FilePath().c_str(), L"")
								);
							message->Attachments().push_back(attachment);

							CreateMockDumpFile(stackWriter->FilePath());
						}
					}

					sender->AddMessage(message);
					return sender;
				}
			};

			TEST(ExceptionEMailerTest, SendFile)
			{
				using namespace Earlgrey::Mail;

				std::tr1::shared_ptr<ExceptionEMailer> exceptionEmailer = Anonymous::GetExceptionEMailer();
				exceptionEmailer->HandleException(NULL);
			}
		}
	}
}
