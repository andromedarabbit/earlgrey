#pragma once
#include "Win32Service.h"
#include "Console.h"

class ServerService : public Earlgrey::Win32Service
{
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

	virtual LPSERVICE_MAIN_FUNCTION ServiceMainFunc() const
	{
		return __super::ServiceMainFunc();
	}
private:
	static BOOL WINAPI ControlHandler(DWORD ctrlType);

private:
	BOOL m_consoleMode;
	Earlgrey::Console m_console;
};