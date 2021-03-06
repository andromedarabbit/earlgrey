#include "stdafx.h"
#include "ServerService.h"

#include "tiostream.h"
#include "ServerConnection.h"
#include "Win32ServiceRunnable.h"
#include "TimeSpan.h"
#include "Console.h"
#include "SingleAppInstance.h"
#include "numeric_cast.hpp"
#include "NetServerEvent.h"
#include "ServerPacketHandler.h"
#include "shared_ptr_helper.h"
#include "IPEndPoint.h"

using namespace Earlgrey;
using namespace Earlgrey::ServiceProcess;

ServerService::ServerService(const Win32ServiceSettings& settings, BOOL consoleMode)
	: ServiceBase(settings.ShortName(), settings.LongName())
	, m_settings(settings) 
	, m_consoleMode(consoleMode)
{
	const WCHAR * serviceName = this->ServiceName().c_str();

	if(m_consoleMode == FALSE)
	{
		if( gSingleAppInstance::Instance().RegisterThisApp(serviceName) == FALSE )
		{
			throw std::invalid_argument("Win32 service instance already exists!");
		}
	}


	EARLGREY_VERIFY(gConsole::Instance().Open(FALSE));
	gConsole::Instance().WindowTitle(this->ServiceName().c_str());

	gConsole::Instance().RedirectStdIO();
	
	// TODO: 어디선가 오류가 나서 clear를 호출해야 한다. 어딘지 찾아야 한다.
	_tcout.clear();
	_tcout << _T("Server started!") << std::endl;

	// 핸들러를 Earlgrey::Console 클래스를 상속 받아 확장하는 식이 나을 것 같다.
	if( SetConsoleCtrlHandler(&ServerService::ControlHandler, TRUE) == FALSE)
	{
		throw std::exception("Couldn't register a service control handler.");
	}

	if( m_settings.AllowOnlyOneInstance()
		&& gSingleAppInstance::Instance().RegisterThisApp(serviceName) == FALSE )
	{
		throw std::invalid_argument("Win32 application instance already exists!");
	}

}

ServerService::~ServerService()
{
	if( SetConsoleCtrlHandler(&ServerService::ControlHandler, FALSE) == FALSE)
	{
		throw std::exception("Couldn't remove a console control handler!");
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
						  , TimeSpan waitHint
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

void ServerService::OnStart(DWORD argc, LPWSTR * argv)
{
	DBG_UNREFERENCED_PARAMETER(argc);
	DBG_UNREFERENCED_PARAMETER(argv);

	// TODO: 포트 번호 등은 ini 파일로 옮기는 게 낫지 않을까?
	const IPAddress2 localAddress(IPAddress2::Any());
	const IPEndPoint localEP(localAddress, 9879);

	//m_server.ExclusiveAddressUse(false);
	//m_server.Initialize( 
	//	std::tr1::shared_ptr<NetServerEvent>(new NetServerEvent())
	//	, std::tr1::shared_ptr<ServerPacketHandler>(new ServerPacketHandler()) 
	//	);
	//if(m_server.Listen( localEP ) == FALSE)
	//{
	//	throw std::exception("");
	//}


	// TODO: 스레드 생성 없이 주 스레드를 대신 쓰게 하자.
	std::tr1::shared_ptr<ThreadRunnable> runnable( static_cast<ThreadRunnable*>( new Win32ServiceRunnable() ));
	EARLGREY_ASSERT(m_serverThread == NULL);
	m_serverThread = Thread::CreateThread( 
		runnable
		, "WindowsRunnable"
		, WIN_MAIN_THREAD_ID 
		);

	EARLGREY_ASSERT(ReportStatus(SERVICE_RUNNING));

	m_serverThread->Join(); // TODO: 이래도 되나?

}

void ServerService::OnStop()
{
	TimeSpan interval(0, 0, 11);
	EARLGREY_VERIFY( 
		ReportStatus(SERVICE_STOP_PENDING, interval)
		);

	m_serverThread->Stop();
}

void ServerService::ProcessUserInput()
{
	_tcin.clear();

	_txstring input;
	// std::getline(_tcin, input, _tcin.widen('\n'));
	std::getline(_tcin, input);
	// _tcin >> input;

	if(input.empty())
		return;
	
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
	instance = static_cast<ServerService*>( ServiceBase::MainService() );
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
			// exit(EXIT_SUCCESS);
			return TRUE;
	}
	return FALSE;

}

