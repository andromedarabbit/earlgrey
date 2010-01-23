#pragma once
#include "Win32Service.h"
#include "Win32ServiceSettings.h"
#include "txstring.h"
#include "xvector.h"

namespace Earlgrey
{
	class Thread;
}


class ServerService : public Earlgrey::Win32Service
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

	// virtual void OnUserInput(const Earlgrey::_txstring& input);
	void RegisterUserInputHandlers(UserInputHandlerPtr handler);

protected:
	virtual BOOL ReportStatus(
		DWORD currentState
		, DWORD waitHint = 3000 /* milliseconds */
		, DWORD errExit = 0
		);

private:
	static BOOL WINAPI ControlHandler(DWORD ctrlType);

	void ProcessUserInput();

private:
	const Win32ServiceSettings& m_settings;
	BOOL m_consoleMode;
	HANDLE m_stopHandle;
	std::tr1::shared_ptr<Earlgrey::Thread> m_serverThread;
	UserInputHandlerConainter m_userInputHandlers;
};