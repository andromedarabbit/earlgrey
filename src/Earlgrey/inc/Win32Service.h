#pragma once 
#include "Uncopyable.h"
#include "txstring.h"
#include "xvector.h"
#include "EventLog.h"

namespace Earlgrey {

	// class EventLog;
	enum PowerBroadcastStatus;

	class Win32Service : private Earlgrey::Uncopyable
	{
		friend class Win32ServiceInstaller;

	public:	// instance methods
		// construction/destruction
		// If <DisplayName> is not set, then it defaults to <ServiceName>.
		explicit Win32Service(
			const TCHAR * serviceName
			, const TCHAR * displayName = NULL
			);
		virtual ~Win32Service();

		// virtual void	OnStart(DWORD argc, LPTSTR * argv) = 0;
		// virtual void	OnStop() = 0;
		virtual void	OnStart(DWORD argc, LPTSTR * argv);
		virtual void	OnStop();
		virtual void	OnPause();
		virtual void	OnContinue();
		virtual void	OnShutdown();

		virtual BOOL OnCustomCommand(int command);
		virtual BOOL OnPowerEvent(PowerBroadcastStatus powerStatus);

		// BOOL InstallService();

	public: // class methods
		static void Run(Win32Service& service);
		static void Run(xvector< std::tr1::shared_ptr<Win32Service> >::Type& services);

	private:
		BOOL ReportStatus(
			DWORD currentState
			, DWORD waitHint = 3000 /* milliseconds */
			, DWORD errExit = 0
			);

	private:		// default handlers
		// The following functions will be used by default.
		// You can provide other handlers. If so, you have to
		// overload several of the "virtual"s above.
		void WINAPI	ServiceCtrl(DWORD ctrlCode);
		void WINAPI	ServiceMain(DWORD argc, LPTSTR * argv);
		// BOOL WINAPI	ControlHandler(DWORD CtrlType);

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

		EventLog m_eventLog;
		SERVICE_STATUS_HANDLE m_serviceStatusHandle;
		SERVICE_STATUS m_serviceStatus;
		DWORD m_checkPoint;
	};
}