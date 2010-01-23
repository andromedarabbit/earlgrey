// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerService.h"
#include "Application.h"
#include "Win32ServiceInstaller.h"
#include "Win32ServiceSettings.h"
#include "EarlgreyProcess.h"
#include "EarlgreyMath.h"

#include "tiostream.h"
#include "StringComparison.hpp"

#include "Environment.h"
#include "Path.h"
#include "File.h"

#include "GlobalExceptionHandler.h"
#include "MiniDump.h"
#include "StackWriter.h"

#include "UserInputHandlers.h"

using namespace std;
using namespace Earlgrey;

namespace 
{
#ifndef UNICODE
#error currently UNICODE should be defined!
#endif

	typedef vector<wstring> ArgContainerType;

	void getArgs(ArgContainerType& args) 
	{
		int argc = 0;
		wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc); // CommandLineToArgvA is not provided.
		if (argv) {
			args.assign(argv, argv + argc);
			LocalFree(argv);
		}
	}

	BOOL IsWin32Service()
	{
		const _tstring parentProcessName(Process::GetParentProcessName(GetCurrentProcessId()));

		StringComparison<STRCMP_CURRENT_CULTURE_IGNORECASE> stringComparer;
		if(
			stringComparer.Equals(_T("services.exe"), parentProcessName.c_str())
			)
		{
			return TRUE;
		}
		return FALSE;
	}

	void RegisterUserInputHandlers(ServerService& service)
	{
		ServerService::UserInputHandlerPtr defaultHandler(new DefaultUserInputHandler());
		service.RegisterUserInputHandlers(defaultHandler);
	}

	DWORD RunAsWin32Service(const Win32ServiceSettings& settings)
	{
		ServerService service(settings, FALSE);
		RegisterUserInputHandlers(service);

		try
		{
			Win32Service::Run(service);
			return EXIT_SUCCESS;
		}
		catch(std::exception&) // 예외 메시지를 어떻게 처리할까?
		{
			return EXIT_FAILURE;
		}
	}

	DWORD RunAsConsoleApplication(const Win32ServiceSettings& settings)
	{
		ServerService service(settings, TRUE);
		RegisterUserInputHandlers(service);

		try
		{
			service.OnStart(__argc, __wargv);
			_tcout << service.ServiceName() << _T(" ends.") << std::endl;
			return EXIT_SUCCESS;
		}
		catch(std::exception&) // 예외 메시지를 어떻게 처리할까?
		{
			return EXIT_FAILURE;
		}
	}


	enum SERVER_RUN_MODE
	{
		SERVER_RUN_MODE_INSTALL
		, SERVER_RUN_MODE_UNINSTALL
	};

	DWORD InstallWin32Service(const Win32ServiceSettings& settings, SERVER_RUN_MODE mode)
	{
		ServerService service(settings, TRUE);
		RegisterUserInputHandlers(service);

		Win32ServiceInstaller installer(service);
		installer.Description(settings.Description());

		if(mode == SERVER_RUN_MODE_INSTALL)
		{
			if(installer.InstallService() == FALSE)
				return EXIT_FAILURE;

			_tcout << _T("서비스 '") << service.ServiceName() << _T("'를 설치했습니다.");
			return EXIT_SUCCESS;
		}

		if(mode == SERVER_RUN_MODE_UNINSTALL)
		{
			if(installer.RemoveService() == FALSE)
				return EXIT_FAILURE;

			_tcout << _T("서비스 '") << service.ServiceName() << _T("'를 제거했습니다.");
			return EXIT_SUCCESS;
		}

		throw std::exception("");
	}

	DWORD InstallWin32Service(const Win32ServiceSettings& settings)
	{
		return InstallWin32Service(settings, SERVER_RUN_MODE_INSTALL);
	}

	DWORD UninstallWin32Service(const Win32ServiceSettings& settings)
	{
		return InstallWin32Service(settings, SERVER_RUN_MODE_UNINSTALL);
	}

	//! ServerService에 멤버 메서드로 넣는 편이 좋겠다. 특히 파일 이름을 하드코딩한 건 문제가 있다.
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
		, _T("사용자 정보 1")
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


int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);


	// const TCHAR * serviceName = _T("EarlgreyServer");
	// const _txstring executableName( Process::MainModuleFileName() );
	// const FileVersionInfo versionInfo( FileVersionInfo::GetVersionInfo(executableName) );
	Win32ServiceSettings settings;

	Application app(settings);
	if(app.InitInstance() == FALSE)
	{
		_tcout << _T("Application initialization failed!");
		return EXIT_FAILURE;
	}

	InitializeGlobalExceptionHandlers();

	// check if it was called by Win32Service daemon
	if(IsWin32Service())
	{
		return RunAsWin32Service(settings);		
	}

	// Run as a console application
	ArgContainerType args;
	getArgs(args);

	for(ArgContainerType::iterator it = args.begin(); it != args.end(); it++)
	{
		_tstring arg = (*it);

		if(arg == _T("-i"))
			return InstallWin32Service(settings);

		if(arg == _T("-u"))
			return UninstallWin32Service(settings);
	}

	return RunAsConsoleApplication(settings);
}

