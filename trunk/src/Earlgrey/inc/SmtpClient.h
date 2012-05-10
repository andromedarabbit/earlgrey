#pragma once
//#include "txstring.h"
#include "MailMessage.h"

#if (_MSC_VER >= 1600)
#	pragma warning(push)
#	pragma warning(disable: 4251 4231)
#endif


namespace Earlgrey
{
	namespace Mail
	{
		class __declspec(dllexport) SmtpClient
		{
		public:
			explicit SmtpClient()
			{

			}

			virtual ~SmtpClient()
			{

			}

			virtual void Send(const MailMessage& message) = NULL;

			inline void Send(const WCHAR * from, const WCHAR * recipient, const WCHAR * subject, const WCHAR * body)
			{
				MailMessage message(from, recipient);
				message.Subject(subject);
				message.Body(body);

				return Send(message);
			}

			virtual void Host(const WCHAR * host) = NULL;
			virtual const WCHAR * Host() = NULL;

			virtual void Port(int port) = NULL;
			virtual const int Port() = NULL;
		};

	} // !Mail
} // !Earlgrey

#if (_MSC_VER >= 1600)
#	pragma warning(pop)
#endif