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
				static _txstring CreateMockDumpFile(const _txstring& dstFile)
				{
					using namespace std;

					const _txstring baseDir = Environment::BaseDirectory();
					const _txstring srcFile( Path::Combine(baseDir, _T("Earlgrey.Test.txt")) );
					// _txstring dstFile( Path::Combine(baseDir, _T("GMailClientTest.cpp")) ); // __FILE__


					BOOL succeeded = File::Exists(srcFile);
					// ASSERT_TRUE2(succeeded);

					if( File::Exists(dstFile.c_str()) )
					{
						succeeded = File::Delete(dstFile);
						// ASSERT_TRUE2(succeeded);
					}

					succeeded = ::CopyFile(srcFile.c_str(), dstFile.c_str(), true);
					// ASSERT_TRUE2(succeeded);

					return dstFile;
				}

				static std::tr1::shared_ptr<ExceptionEMailer> GetExceptionEMailer()
				{
					using namespace Earlgrey::Mail;

					DefaultAppSettings appSettings;

					const _txstring miniDumpClassName(_T("Earlgrey::MiniDump"));
					const _txstring stackWriterClassName(_T("Earlgrey::StackWriter"));

					// 임시 코드
					const TCHAR * username = _T("projectearlgrey@gmail.com");
					const TCHAR * password = _T("dkswjsgkwldksgdmsdkagh");
					MailMessage::MailAddressPtr from(new MailAddress(_T("projectearlgrey@gmail.com"), _T("빌드 마스터")));
					MailMessage::MailAddressPtr to1(new MailAddress(_T("projectearlgrey@gmail.com"), _T("최재훈")));
					_txstring subject(_T("[MiniDump] "));
					subject += appSettings.ShortName();
					_txstring body(_T("Please see attachments!"));

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
								new Attachment(miniDump->FilePath().c_str(), _T(""))
								);
							message->Attachments().push_back(attachment);

							CreateMockDumpFile(miniDump->FilePath());
						}

						if(handler->ClassName() == stackWriterClassName)
						{
							const StackWriter * stackWriter = static_cast<StackWriter*>(handler.get());

							MailMessage::AttachmentPtr attachment(
								new Attachment(stackWriter->FilePath().c_str(), _T(""))
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
