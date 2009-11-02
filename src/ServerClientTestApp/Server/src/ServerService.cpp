#include "stdafx.h"
#include "ServerService.h"

#include "tiostream.h"
// #include "ServerInit.h"
#include "ServerConnection.h"
#include "WindowsRunnable.h"
#include "AppInitializer.h"

using namespace Earlgrey;


ServerService::ServerService(
   const TCHAR * serviceName
   , const TCHAR * displayName 
   , BOOL consoleMode
   )
   : Win32Service(serviceName, displayName)
   , m_console()
   , m_consoleMode(consoleMode)
{
	if(m_consoleMode)
	{
		EARLGREY_VERIFY(m_console.Open());
		m_console.WindowTitle(_T("Earlgrey Server"));

		m_console.WriteLine(_T("Console::WriteLine() works!"));

		m_console.RedirectStdIO();
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
	if(m_consoleMode)
	{
		m_console.Close();
	}

}


void ServerService::OnStart(DWORD argc, LPTSTR * argv)
{
	AppInfo app;
	app.InitInstance(AppType::E_APPTYPE_DEFAULT);

	//ServerCreate(100);
	Acceptor<ServerConnection>* acceptor = new Acceptor<ServerConnection>(100);
	acceptor->Initialize();

	std::tr1::shared_ptr<Thread> serverThread = Thread::CreateThread( 
		std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new WindowsRunnable())), "WindowsRunnable" );

	serverThread->Join();

}

void ServerService::OnStop()
{

}

BOOL WINAPI ServerService::ControlHandler(DWORD ctrlType)
{
	_txstring input;
	switch( ctrlType ) {
		case CTRL_C_EVENT:
		case CTRL_BREAK_EVENT:  // use Ctrl+C or Ctrl+Break to simulate			
			// Process User Input
			_tcin.clear();
			std::getline<TCHAR>(_tcin, input);
			return TRUE;

		case CTRL_CLOSE_EVENT:
		case CTRL_LOGOFF_EVENT:
		case CTRL_SHUTDOWN_EVENT:
			// _tcout << TEXT("Stopping ") << m_displayName << std::endl;
			// OnStop();
			exit(EXIT_SUCCESS);
	}
	return FALSE;

}

