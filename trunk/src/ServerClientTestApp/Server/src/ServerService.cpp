#include "stdafx.h"
#include "ServerService.h"

#include "tiostream.h"
// #include "ServerInit.h"
#include "ServerConnection.h"
#include "Win32ServiceRunnable.h"
#include "AppInitializer.h"
#include "TimeSpan.h"
#include "Console.h"

using namespace Earlgrey;


ServerService::ServerService(
   const TCHAR * serviceName
   , const TCHAR * displayName 
   , BOOL consoleMode
   )
   : Win32Service(serviceName, displayName)
   // , m_console()
   , m_consoleMode(consoleMode)
   , m_stopHandle(NULL)
{
	if(m_consoleMode)
	{
		EARLGREY_VERIFY(gConsole::Instance().Open(FALSE));
		gConsole::Instance().WindowTitle(serviceName);

		gConsole::Instance().RedirectStdIO();
		
		// TODO: 어디선가 오류가 나서 clear를 호출해야 한다. 어딘지 찾아야 한다.
		_tcout.clear();
		_tcout << _T("Server started!") << std::endl;

		// 핸들러를 Earlgrey::Console 클래스를 상속 받아 확장하는 식이 나을 것 같다.
		if( SetConsoleCtrlHandler(&ServerService::ControlHandler, TRUE) == FALSE)
		{
			// TODO
		}
	}
}

ServerService::~ServerService()
{
	_tcout << std::endl << std::endl
		<< _T("Enter to end") << std::endl;

	_txstring lastInput;
	std::getline<TCHAR>(_tcin, lastInput);


	if( SetConsoleCtrlHandler(&ServerService::ControlHandler, FALSE) == FALSE)
	{
		// TODO
	}

	if(m_consoleMode)
	{
		gConsole::Instance().Close();
	}

}

BOOL ServerService::ReportStatus(
						  DWORD currentState
						  , DWORD waitHint
						  , DWORD errExit
						  )
{
	if(m_consoleMode)
	{
		_tcout << _T("[STATUS REPORT] ") << currentState 
			<< _T(", ") << waitHint
			<< _T(", ") << errExit
			<< std::endl;
		return TRUE;
	}

	return __super::ReportStatus(currentState, waitHint, errExit);
}

void ServerService::OnStart(DWORD argc, LPTSTR * argv)
{
	// ::DebugBreak();
	m_stopHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	EARLGREY_VERIFY(m_stopHandle);


	AppInfo app;
	app.InitInstance(AppType::E_APPTYPE_DEFAULT);

	//! \todo delete 안 해도 되나?
	ServerConnection* connection = new ServerConnection();
	connection->Accept(100);

	std::tr1::shared_ptr<ServerService> thisService(this);
	std::tr1::shared_ptr<IRunnable> runnable( static_cast<IRunnable*>( new WindowsRunnable(thisService) ));
	std::tr1::shared_ptr<Thread> serverThread = Thread::CreateThread( runnable, "WindowsRunnable" );

	EARLGREY_ASSERT(ReportStatus(SERVICE_RUNNING));

	serverThread->Join();

}

void ServerService::OnStop()
{
	TimeSpan interval(0, 0, 11);
	EARLGREY_VERIFY( 
		ReportStatus(SERVICE_STOP_PENDING, interval.Milliseconds())
		);

	EARLGREY_VERIFY(::SetEvent(m_stopHandle));
}

void ServerService::ProcessUserInput()
{
	_tcin.clear();

	_txstring input;
	std::getline<TCHAR>(_tcin, input);

	OnUserInput(input);
}

void ServerService::OnUserInput(_txstring& input)
{
	if(input == _T("stop"))
		OnStop();
}

BOOL WINAPI ServerService::ControlHandler(DWORD ctrlType)
{
	ServerService* instance = NULL;
	instance = static_cast<ServerService*>( Win32Service::MainService() );
	EARLGREY_ASSERT(instance != NULL);

	switch( ctrlType ) {
		case CTRL_BREAK_EVENT:  // use Ctrl+Break to simulate			
			// Process User Input
			instance->ProcessUserInput();			
			return TRUE;

		case CTRL_C_EVENT:
		case CTRL_CLOSE_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			// _tcout << TEXT("Stopping ") << m_displayName << std::endl;
			instance->OnStop();
			exit(EXIT_SUCCESS);
	}
	return FALSE;

}

