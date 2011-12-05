// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Application.h"
#include "ServerService.h"
#include "ServiceInstaller.h"
#include "Win32ServiceSettings.h"
#include "ServiceHelper.h"

#include "tiostream.h"
#include "UserInputHandlers.h"

#include "Shellapi.h" // CommandLineToArgvW
#include "shared_ptr_helper.h"

using namespace std;
using namespace Earlgrey;
using namespace Earlgrey::ServiceProcess;

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
			ServiceBase::Run(service);
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

		ServiceInstaller installer(service);
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

	Win32ServiceSettings settings;

	Application app(settings);
	if(app.InitInstance() == FALSE)
	{
		_tcout << _T("Application initialization failed!");
		return EXIT_FAILURE;
	}

	// check if it was called by Win32Service daemon
	if(Helper::RunByWin32Service())
	{
		return RunAsWin32Service(settings);		
	}

	// Run as a console application
	ArgContainerType args;
	getArgs(args);

	{
		std::tr1::shared_ptr<int> i(new int());
	}

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

