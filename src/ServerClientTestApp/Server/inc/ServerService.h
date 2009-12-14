#pragma once
#include "Win32Service.h"
#include "Win32ServiceSettings.h"
#include "txstring.h"

namespace Earlgrey
{
	class Thread;
}


class ServerService : public Earlgrey::Win32Service
{
	friend class Win32ServiceRunnable;

public:
	explicit ServerService(const Win32ServiceSettings& settings, BOOL consoleMode);
	
	virtual ~ServerService();

	virtual void	OnStart(DWORD argc, LPTSTR * argv);
	virtual void	OnStop();

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
	const Win32ServiceSettings& m_settings;
	BOOL m_consoleMode;
	HANDLE m_stopHandle;
	std::tr1::shared_ptr<Earlgrey::Thread> m_serverThread;
};