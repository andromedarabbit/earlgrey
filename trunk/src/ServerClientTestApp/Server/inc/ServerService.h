#pragma once
#include "Win32Service.h"
#include "Console.h"

class ServerService : public Earlgrey::Win32Service
{

public:
	explicit ServerService(
		const TCHAR * serviceName
		, const TCHAR * displayName = NULL
		);

	virtual ~ServerService();

	virtual void	OnStart(DWORD argc, LPTSTR * argv);
	virtual void	OnStop();
	// virtual void	OnPause();
	// virtual void	OnContinue();
	// virtual void	OnShutdown();
private:
	Earlgrey::Console m_console;
};