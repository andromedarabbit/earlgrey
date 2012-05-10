#include "stdafx.h"
#define EXP_STL
#include "GMailClient.h"
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

				MailMessage::MailAddressPtr from(new MailAddress(L"projectearlgrey@gmail.com", L"������"));
				MailMessage::MailAddressPtr to1(new MailAddress(L"earlgreyproject@googlegroups.com", L"��׷���"));

				Earlgrey::Mail::MailMessage msg(from, to1);
				msg.Subject(L"�����׽�Ʈ GMailClientTest::Send");
				msg.Body(L"������ ���� �׽�Ʈ");

				GMailClient client(L"projectearlgrey@gmail.com", L"dkswjsgkwldksgdmsdkagh");
				ASSERT_NO_THROW(client.Send(msg));				
			}


			TEST(GMailClientTest, SendFile)
			{
				struct Anonymous
				{
					static xwstring CreateMockDumpFile()
					{
						using namespace std;

						const xwstring baseDir = Environment::BaseDirectoryW();
						const xwstring srcFile( Path::Combine(baseDir, L"Earlgrey.Test.txt") );
						xwstring dstFile( Path::Combine(baseDir, L"GMailClientTest.cpp") ); // __FILE__


						BOOL succeeded = File::Exists(srcFile);
						// ASSERT_TRUE2(succeeded);

						if( File::Exists(dstFile.c_str()) )
						{
							succeeded = File::Delete(dstFile);
							// ASSERT_TRUE2(succeeded);
						}

						succeeded = File::Copy(srcFile, dstFile, TRUE);
						// ASSERT_TRUE2(succeeded);

						return dstFile;
					}
				};


				using namespace Earlgrey::Mail;

				MailMessage::MailAddressPtr from(new MailAddress(L"projectearlgrey@gmail.com", L"������"));
				MailMessage::MailAddressPtr to1(new MailAddress(L"earlgreyproject@googlegroups.com", L"��׷���"));
				// MailMessage::MailAddressPtr to1(new MailAddress(L"projectearlgrey@gmail.com", L"��׷���"));

				Earlgrey::Mail::MailMessage msg(from, to1);
				msg.Subject(L"GMailClientTest::SendFile");
				msg.Body(L"�����Ϸ� ���� ������ �׽�Ʈ");


				xwstring filePath = Anonymous::CreateMockDumpFile();

				MailMessage::AttachmentPtr attachment(
					// new Attachment(filePath.c_str(), L"text/plain")
					new Attachment(filePath.c_str(), L"")
					);
				msg.Attachments().push_back(attachment);

				GMailClient client(L"projectearlgrey@gmail.com", L"dkswjsgkwldksgdmsdkagh");

				ASSERT_NO_THROW(client.Send(msg));
			}
		}
	}
}