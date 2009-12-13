#include "stdafx.h"
#include "ServerService.h"

#include "tiostream.h"
#include "ServerConnection.h"
#include "Win32ServiceRunnable.h"
#include "AppInitializer.h"
#include "TimeSpan.h"
#include "Console.h"

#include "GlobalExceptionHandler.h"
#include "MiniDump.h"
#include "StackWriter.h"

#include "Environment.h"
#include "Path.h"
#include "File.h"

using namespace Earlgrey;

namespace 
{
	//! ServerService�� ��� �޼���� �ִ� ���� ���ڴ�. Ư�� ���� �̸��� �ϵ��ڵ��� �� ������ �ִ�.
	void RegisterMiniDump()
	{
		const _txstring baseDir = Environment::BaseDirectory();
		const _txstring filePath( Path::Combine(baseDir, _T("MiniDump.dmp")) );

		if( File::Exists(filePath) )
		{
			EARLGREY_ASSERT( File::Delete(filePath) );
		}

		const MINIDUMP_TYPE dumpType = MiniDumpNormal;

		std::tr1::shared_ptr<UnhandledExceptionHandler> miniDump( 
			new MiniDump(filePath.c_str(), dumpType) 
			);
		EARLGREY_ASSERT(miniDump != NULL);

		/*
		miniDump->AddExtendedMessage(
			static_cast<MINIDUMP_STREAM_TYPE>(LastReservedStream + 1)
			, _T("����� ���� 1")
			);
			*/

		GlobalExceptionHandler::Register(miniDump);
	}

	void RegisterStackWriter()
	{
		const _txstring baseDir = Environment::BaseDirectory();
		const _txstring filePath( Path::Combine(baseDir, _T("StackWriter.txt")) );

		if( File::Exists(filePath) )
		{
			EARLGREY_ASSERT( File::Delete(filePath) );
		}

		std::tr1::shared_ptr<UnhandledExceptionHandler> stackWriter( 
			new StackWriter(filePath, StackWalker::OptionsAll) 
			);
		EARLGREY_ASSERT(stackWriter != NULL);

		GlobalExceptionHandler::Register(stackWriter);
		
	}

	void InitializeGlobalExceptionHandlers()
	{
		GlobalExceptionHandler::Initialize();
		RegisterMiniDump();
		RegisterStackWriter();
	}
}

ServerService::ServerService(
   const TCHAR * serviceName
   , const TCHAR * displayName 
   , BOOL consoleMode
   )
   : Win32Service(serviceName, displayName)
   , m_consoleMode(consoleMode)
   , m_stopHandle(NULL)
{
	if(m_consoleMode)
	{
		EARLGREY_VERIFY(gConsole::Instance().Open(FALSE));
		gConsole::Instance().WindowTitle(serviceName);

		gConsole::Instance().RedirectStdIO();
		
		// TODO: ��𼱰� ������ ���� clear�� ȣ���ؾ� �Ѵ�. ����� ã�ƾ� �Ѵ�.
		_tcout.clear();
		_tcout << _T("Server started!") << std::endl;

		// �ڵ鷯�� Earlgrey::Console Ŭ������ ��� �޾� Ȯ���ϴ� ���� ���� �� ����.
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


	AppInfo app;
	if(app.InitInstance(AppType::E_APPTYPE_DEFAULT) == FALSE)
		throw std::exception("Application initialization failed!");

	InitializeGlobalExceptionHandlers();

	//! \todo delete �� �ص� �ǳ�?
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
			instance->OnStop();
			exit(EXIT_SUCCESS);
	}
	return FALSE;

}

