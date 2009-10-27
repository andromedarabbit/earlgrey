#pragma once 
#include "Win32Service.h"
#include "Uncopyable.h"
#include "txstring.h"
// #include "SingleAppInstance.h"

namespace Earlgrey {
//	class SingleAppInstance;
//}
// class Earlgrey::SingleAppInstance;
	enum PowerBroadcastStatus;

	class Win32Service : private Earlgrey::Uncopyable
	{

	public:		// construction/destruction
		// If <DisplayName> is not set, then it defaults to <ServiceName>.
		explicit Win32Service(
			const TCHAR * serviceName
			, const TCHAR * displayName = NULL
			);
		virtual ~Win32Service();

		virtual void	Run(DWORD argc, LPTSTR * argv) = 0;
		virtual void	Stop() = 0;
		virtual void	Pause();
		virtual void	Continue();
		virtual void	Shutdown();

		virtual BOOL OnPowerEvent(PowerBroadcastStatus powerStatus);

		BOOL InstallService();
	private:
		// Earlgrey::SingleAppInstance m_singleAppInstance;

		Earlgrey::_txstring m_serviceName;
		Earlgrey::_txstring m_displayName;

		// parameters to the "CreateService()" function:
		DWORD			m_dwDesiredAccess;		// default: SERVICE_ALL_ACCESS
		DWORD			m_dwServiceType;		// default: SERVICE_WIN32_OWN_PROCESS
		DWORD			m_dwStartType;			// default: SERVICE_AUTO_START
		DWORD			m_dwErrorControl;		// default: SERVICE_ERROR_NORMAL
		LPCTSTR			m_pszLoadOrderGroup;	// default: NULL
		DWORD			m_dwTagID;				// retrieves the tag identifier
		LPCTSTR			m_pszDependencies;		// default: NULL
		LPCTSTR			m_pszStartName;			// default: NULL
		LPCTSTR			m_pszPassword;			// default: NULL
	};
}