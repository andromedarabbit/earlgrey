#include "stdafx.h"
#include "GmailClient.h"
#include "MailMessage.h"
#include "Environment.h"
#include "File.h"
#include "Path.h"


namespace Earlgrey
{
	namespace Extension
	{
		namespace Test
		{
			TEST(GMailClientTest, Send)
			{
				using namespace Earlgrey::Mail;

				MailMessage::MailAddressPtr from(new MailAddress(_T("kaistizen@gmail.com"), _T("최재훈")));
				MailMessage::MailAddressPtr to1(new MailAddress(_T("earlgreyproject@googlegroups.com"), _T("얼그레이")));

				Earlgrey::Mail::MailMessage msg(from, to1);
				msg.Subject(_T("단위테스트 GMailClientTest::Send"));
				msg.Body(_T("지메일 전송 테스트"));

				GMailClient client(_T("buildmaster@kaistizen.net"), _T("dkswjsgkwldksgdmsdkagh"));
				ASSERT_NO_THROW(client.Send(msg));				
			}


			TEST(GMailClientTest, SendFile)
			{
				struct Anonymous
				{
					static _txstring CreateMockDumpFile()
					{
						using namespace std;

						const _txstring baseDir = Environment::BaseDirectory();
						const _txstring srcFile( Path::Combine(baseDir, _T("Earlgrey.Test.txt")) );
						_txstring dstFile( Path::Combine(baseDir, _T("GMailClientTest.cpp")) ); // __FILE__


						BOOL succeeded = File::Exists(srcFile);
						// ASSERT_TRUE(succeeded);

						if( File::Exists(dstFile.c_str()) )
						{
							succeeded = File::Delete(dstFile);
							// ASSERT_TRUE(succeeded);
						}

						succeeded = File::Copy(srcFile, dstFile, TRUE);
						// ASSERT_TRUE(succeeded);

						return dstFile;
					}
				};


				using namespace Earlgrey::Mail;

				MailMessage::MailAddressPtr from(new MailAddress(_T("kaistizen@gmail.com"), _T("최재훈")));
				MailMessage::MailAddressPtr to1(new MailAddress(_T("earlgreyproject@googlegroups.com"), _T("얼그레이")));
				// MailMessage::MailAddressPtr to1(new MailAddress(_T("kaistizen@gmail.com"), _T("얼그레이")));

				Earlgrey::Mail::MailMessage msg(from, to1);
				msg.Subject(_T("GMailClientTest::SendFile"));
				msg.Body(_T("지메일로 파일 보내기 테스트"));


				_txstring filePath = Anonymous::CreateMockDumpFile();

				MailMessage::AttachmentPtr attachment(
					// new Attachment(filePath.c_str(), _T("text/plain"))
					new Attachment(filePath.c_str(), _T(""))
					);
				msg.Attachments().push_back(attachment);

				GMailClient client(_T("buildmaster@kaistizen.net"), _T("dkswjsgkwldksgdmsdkagh"));

				ASSERT_NO_THROW(client.Send(msg));
			}
		}
	}
}