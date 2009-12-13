#pragma once
#include "Win32Service.h"
#include "txstring.h"

namespace Earlgrey
{
	class Thread;
}

class ServerService : public Earlgrey::Win32Service
{
	friend class Win32ServiceRunnable;

public:
	explicit ServerService(
		const TCHAR * serviceName
		, const TCHAR * displayName = NULL
		, BOOL consoleMode = FALSE
		);

	virtual ~ServerService();

	virtual void	OnStart(DWORD argc, LPTSTR * argv);
	virtual void	OnStop();
	// virtual void	OnPause();
	// virtual void	OnContinue();
	// virtual void	OnShutdown();

	//virtual LPSERVICE_MAIN_FUNCTION ServiceMainFunc() const
	//{
	//	return __super::ServiceMainFunc();
	//}

	virtual void OnUserInput(Earlgrey::_txstring& input);

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
	BOOL m_consoleMode;
	HANDLE m_stopHandle;
	std::tr1::shared_ptr<Earlgrey::Thread> m_serverThread;
};