#include "stdafx.h"
#include "ServerService.h"

#include "tiostream.h"
#include "ServerConnection.h"
#include "Win32ServiceRunnable.h"
#include "TimeSpan.h"
#include "Console.h"


using namespace Earlgrey;

ServerService::ServerService(const Win32ServiceSettings& settings, BOOL consoleMode)
	: Win32Service(settings.ShortName(), settings.LongName())
	, m_settings(settings) 
	, m_consoleMode(consoleMode)
	, m_stopHandle(NULL)
{
	if(m_consoleMode)
	{
		EARLGREY_VERIFY(gConsole::Instance().Open(FALSE));
		gConsole::Instance().WindowTitle(this->ServiceName().c_str());

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
	if( SetConsoleCtrlHandler(&ServerService::ControlHandler, FALSE) == FALSE)
	{
		// TODO
	}

	if(m_consoleMode)
	{
		_tcout << std::endl << std::endl
			<< _T("Enter to end") << std::endl;

		_txstring lastInput;
		std::getline<TCHAR>(_tcin, lastInput);

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
	DBG_UNREFERENCED_PARAMETER(argc);
	DBG_UNREFERENCED_PARAMETER(argv);


	m_stopHandle = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	EARLGREY_VERIFY(m_stopHandle);

	//! \todo delete 안 해도 되나?
	ServerConnection* connection = new ServerConnection();
	connection->Accept(100);

	std::tr1::shared_ptr<ThreadRunnable> runnable( static_cast<ThreadRunnable*>( new Win32ServiceRunnable(m_stopHandle) ));
	EARLGREY_ASSERT(m_serverThread == NULL);
	m_serverThread = Thread::CreateThread( 
		runnable
		, "WindowsRunnable"
		, WIN_MAIN_THREAD_ID 
		);

	EARLGREY_ASSERT(ReportStatus(SERVICE_RUNNING));

	m_serverThread->Join();

}

void ServerService::OnStop()
{
	TimeSpan interval(0, 0, 11);
	EARLGREY_VERIFY( 
		ReportStatus(SERVICE_STOP_PENDING, interval.Milliseconds())
		);

	m_serverThread->Stop();
	// EARLGREY_VERIFY(::SetEvent(m_stopHandle));
}

void ServerService::ProcessUserInput()
{
	_tcin.clear();

	_txstring input;
	std::getline<TCHAR>(_tcin, input);

	// OnUserInput(input);
	UserInputHandlerConainter::iterator it = m_userInputHandlers.begin();
	for( ; it != m_userInputHandlers.end(); it++)
	{
		(*it)->OnUserInput(*this, input);
	}
}

void ServerService::RegisterUserInputHandlers(UserInputHandlerPtr handler)
{
	m_userInputHandlers.push_back(handler);
}
/*
void ServerService::OnUserInput(const _txstring& input)
{
	if(input == _T("stop"))
		OnStop();
}
*/

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
			instance->OnStop();
			exit(EXIT_SUCCESS);
	}
	return FALSE;

}

