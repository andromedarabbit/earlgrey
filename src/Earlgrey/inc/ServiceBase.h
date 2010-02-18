#pragma once 
#include "Uncopyable.h"
#include "txstring.h"
#include "xvector.h"
#include "EventLog.h"

namespace Earlgrey {

	namespace ServiceProcess
	{

		// class EventLog;
		enum PowerBroadcastStatus;

		class ServiceBase : private Earlgrey::Uncopyable
		{
			friend class ServiceInstaller;

		public:	// instance methods
			// construction/destruction
			// If <DisplayName> is not set, then it defaults to <ServiceName>.
			explicit ServiceBase(
				const TCHAR * serviceName
				, const TCHAR * displayName = NULL
				);
			virtual ~ServiceBase();

			// TODO 나중에 돌려놓자
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
			virtual LPSERVICE_MAIN_FUNCTION ServiceMainFunc() const;
			

			inline const _txstring& ServiceName() const
			{
				return m_serviceName;
			}

		public: // class methods
			static void Run(ServiceBase& service);
			// static void Run(xvector< std::tr1::shared_ptr<Win32Service> >::Type& services);

		protected:
			static ServiceBase * MainService() 
			{
				return MAIN_SERVICE;
			}
		
			virtual BOOL ReportStatus(
				DWORD currentState
				, DWORD waitHint = 3000 /* milliseconds */
				, DWORD errExit = 0
				);

			void WriteEventLog(
				const TCHAR * message
				, WORD eventType = EVENTLOG_INFORMATION_TYPE
				, DWORD eventID = 0
				, WORD category = 0 
				);
		private:
			void Start(DWORD argc, LPTSTR * argv);
			DWORD ControlHandler(
				DWORD    dwControl,
				DWORD    dwEventType,
				LPVOID   lpEventData
				);

		private:		// default handlers
			// The following functions will be used by default.
			// You can provide other handlers. If so, you have to
			// overload several of the "virtual"s above.
			static DWORD WINAPI ServiceCtrl(
				DWORD    dwControl,
				DWORD    dwEventType,
				LPVOID   lpEventData,
				LPVOID   lpContext
				);
			static void WINAPI	ServiceMain(DWORD argc, LPTSTR * argv);
			// BOOL WINAPI	ControlHandler(DWORD CtrlType);

		private:
			static ServiceBase* MAIN_SERVICE;
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
}