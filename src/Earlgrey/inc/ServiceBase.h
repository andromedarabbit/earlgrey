#pragma once 
#include "Uncopyable.h"
#include "txstring.h"
#include "xvector.h"
#include "EventLog.h"
#include "TimeSpan.h"

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
				const WCHAR * serviceName
				, const WCHAR * displayName = NULL
				);
			virtual ~ServiceBase();

			// TODO 나중에 돌려놓자
			// virtual void	OnStart(DWORD argc, LPWSTR * argv) = 0;
			// virtual void	OnStop() = 0;
			virtual void	OnStart(DWORD argc, LPWSTR * argv);
			virtual void	OnStop();
			virtual void	OnPause();
			virtual void	OnContinue();
			virtual void	OnShutdown();

			virtual BOOL OnCustomCommand(int command);
			virtual BOOL OnPowerEvent(PowerBroadcastStatus powerStatus);

			// BOOL InstallService();
			virtual LPSERVICE_MAIN_FUNCTIONW ServiceMainFunc() const;
			

			inline const xwstring& ServiceName() const
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
				, TimeSpan waitHint = TimeSpan::FromSeconds(3)
				, DWORD errExit = 0
				);

			void WriteEventLog(
				const WCHAR * message
				, WORD eventType = EVENTLOG_INFORMATION_TYPE
				, DWORD eventID = 0
				, WORD category = 0 
				);
		private:
			void Start(DWORD argc, LPWSTR * argv);
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
			static void WINAPI	ServiceMain(DWORD argc, LPWSTR * argv);
			// BOOL WINAPI	ControlHandler(DWORD CtrlType);

		private:
			static ServiceBase* MAIN_SERVICE;
			// Earlgrey::SingleAppInstance m_singleAppInstance;

			Earlgrey::xwstring m_serviceName;
			Earlgrey::xwstring m_displayName;

			// parameters to the "CreateService()" function:
			DWORD			m_dwDesiredAccess;		// default: SERVICE_ALL_ACCESS
			DWORD			m_dwServiceType;		// default: SERVICE_WIN32_OWN_PROCESS
			DWORD			m_dwStartType;			// default: SERVICE_AUTO_START
			DWORD			m_dwErrorControl;		// default: SERVICE_ERROR_NORMAL
			LPCWSTR			m_pszLoadOrderGroup;	// default: NULL
			DWORD			m_dwTagID;				// retrieves the tag identifier
			LPCWSTR			m_pszDependencies;		// default: NULL
			LPCWSTR			m_pszStartName;			// default: NULL
			LPCWSTR			m_pszPassword;			// default: NULL

			EventLog m_eventLog;
			SERVICE_STATUS_HANDLE m_serviceStatusHandle;
			SERVICE_STATUS m_serviceStatus;
			DWORD m_checkPoint;
		};
	}
}