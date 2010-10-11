#pragma once
#include "ServiceBase.h"
#include "Win32ServiceSettings.h"
#include "txstring.h"
#include "xvector.h"
#include "TimeSpan.h"
#include "Listener.h"

namespace Earlgrey
{
	class Thread;
	// class Server;
}


class ServerService : public Earlgrey::ServiceProcess::ServiceBase
{
	friend class Win32ServiceRunnable;

public:
	class UserInputHandler : private Earlgrey::Uncopyable
	{
	public:
		virtual void OnUserInput(ServerService& service, const Earlgrey::_txstring& input) = NULL;
	};
	
	typedef std::tr1::shared_ptr<UserInputHandler> UserInputHandlerPtr;
	typedef Earlgrey::xvector<UserInputHandlerPtr>::Type UserInputHandlerConainter;

public:
	explicit ServerService(const Win32ServiceSettings& settings, BOOL consoleMode);
	virtual ~ServerService();

	virtual void	OnStart(DWORD argc, LPTSTR * argv);
	virtual void	OnStop();

	void RegisterUserInputHandlers(UserInputHandlerPtr handler);

protected:
	virtual BOOL ReportStatus(
		DWORD currentState
		, Earlgrey::TimeSpan waitHint = Earlgrey::TimeSpan::FromSeconds(3)
		, DWORD errExit = EXIT_SUCCESS
		);

private:
	static BOOL WINAPI ControlHandler(DWORD ctrlType);

	void ProcessUserInput();

private:
	const Win32ServiceSettings& m_settings;
	BOOL m_consoleMode;
	std::tr1::shared_ptr<Earlgrey::Thread> m_serverThread;
	UserInputHandlerConainter m_userInputHandlers;
	Earlgrey::Server m_server;
};