#pragma once
#include <WinSock2.h>
#include <Windows.h>
#include <tchar.h>

#include "tstring.h"
#include "SmtpClient.h"

#if (_MSC_VER >= 1600)
#	pragma warning(push)
#	pragma warning(disable: 4251 4231)
#endif



//  TODO: ��ݿ��� C# �̰� �����ڵ带  ���ϱ� TCHAR �� �ƴ� WCHAR �� ���� �� ���ڴ�.
namespace Earlgrey
{
	namespace Extension
	{
		class __declspec(dllexport) GMailClient : public Earlgrey::Mail::SmtpClient
		{
		public:
			explicit GMailClient(const WCHAR * username, const WCHAR * password);
			virtual ~GMailClient();

			virtual void Send(const Earlgrey::Mail::MailMessage& msg);

			virtual void Host(const WCHAR * host);
			virtual const WCHAR * Host();

			virtual void Port(int port);
			virtual const int Port();

		private:
			std::wstring m_Host;
			std::wstring m_UserName;
			std::wstring m_Password;
			int m_Port;
		};
	}
}

#if (_MSC_VER >= 1600)
#	pragma warning(pop)
#endif