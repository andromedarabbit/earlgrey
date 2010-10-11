#include "GMailClient.h"
#include "MarshalAs.h"

using namespace System;
using namespace System::Net::Mail;
using namespace msclr::interop;

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
				String^ smtpServerAddress = marshal_as<String^>(m_Host.c_str());
				String^ username = marshal_as<String^>(m_UserName.c_str());
				String^ password = marshal_as<String^>(m_Password.c_str());


				System::Net::Mail::SmtpClient^ client = gcnew System::Net::Mail::SmtpClient(smtpServerAddress, m_Port);
				client->UseDefaultCredentials = false; // �ý��ۿ� ������ ���� ������ ������� �ʴ´�.
				client->EnableSsl = true;  // SSL�� ����Ѵ�.
				client->DeliveryMethod = SmtpDeliveryMethod::Network; // �̰� ���� ������ Gmail�� ������ ���� ���Ѵ�.
				client->Credentials = gcnew System::Net::NetworkCredential(username, password);

				MailMessage^ message = marshal_as<MailMessage^>(msg);
				message->BodyEncoding = System::Text::Encoding::UTF8;
				message->SubjectEncoding = System::Text::Encoding::UTF8;

				// ����� ������ ������.
				client->Send(message);

				// Clean up.
				delete message; // Dispose
			}
			catch (Exception^ ex)
			{
				marshal_context context;		
				const char * exceptionMsg = context.marshal_as<const char *>(ex->Message);
				throw std::exception(exceptionMsg);
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
	} // !Extension
} // !Earlgrey
