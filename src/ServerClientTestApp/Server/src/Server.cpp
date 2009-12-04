// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerService.h"
#include "Win32ServiceInstaller.h"
#include "txstring.h"
#include "xvector.h"
#include "EarlgreyProcess.h"
#include "FileVersionInfo.h"
#include "EarlgreyMath.h"

#include "tiostream.h"
#include "StringComparison.hpp"

#ifndef UNICODE
#error currently UNICODE should be defined!
#endif


using namespace std;
using namespace Earlgrey;

namespace 
{
	typedef xvector<xwstring>::Type ArgContainerType;

	void getArgs(ArgContainerType& args) {
		int argc = 0;
		wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
		if (argv) {
			args.assign(argv, argv + argc);
			LocalFree(argv);
		}
	}

	enum SERVER_RUN_MODE
	{
		SERVER_RUN_MODE_INSTALL
		, SERVER_RUN_MODE_UNINSTALL
	};

	DWORD RunAsWin32Service(const TCHAR * serviceName, const FileVersionInfo& versionInfo)
	{
		ServerService service(
			serviceName
			, versionInfo.ProductName().c_str()
			, FALSE
			);

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

	DWORD RunAsConsoleApplication(const TCHAR * serviceName, const FileVersionInfo& versionInfo)
	{
		ServerService service(
			serviceName
			, versionInfo.ProductName().c_str()
			, TRUE
			);

		service.OnStart(__argc, __wargv);
		_tcout << service.ServiceName() << _T(" ends.") << std::endl;
		return EXIT_SUCCESS;
	}


	DWORD InstallWin32Service(const TCHAR * serviceName, const FileVersionInfo& versionInfo, SERVER_RUN_MODE mode)
	{
		ServerService service(
			serviceName
			, versionInfo.ProductName().c_str()
			, TRUE
			);

		Win32ServiceInstaller installer(service);
		installer.Description(versionInfo.FileDescription());

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

	DWORD InstallWin32Service(const TCHAR * serviceName, const FileVersionInfo& versionInfo)
	{
		return InstallWin32Service(serviceName, versionInfo, SERVER_RUN_MODE_INSTALL);
	}

	DWORD UninstallWin32Service(const TCHAR * serviceName, const FileVersionInfo& versionInfo)
	{
		return InstallWin32Service(serviceName, versionInfo, SERVER_RUN_MODE_UNINSTALL);
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


	const TCHAR * serviceName = _T("EarlgreyServer");

	const _txstring executableName = Process::MainModuleFileName();
	const FileVersionInfo versionInfo( FileVersionInfo::GetVersionInfo(executableName) );

	// check if it was called by Win32Service daemon
	const _txstring parentProcessName(Process::GetParentProcessName(GetCurrentProcessId()));

	StringComparison<STRCMP_CURRENT_CULTURE_IGNORECASE> stringComparer;
	if(
		stringComparer.Equals(_T("services.exe"), parentProcessName.c_str())
		)
	{
		return RunAsWin32Service(serviceName, versionInfo);		
	}

	// Run as a console application
	ArgContainerType args;
	getArgs(args);

	for(ArgContainerType::iterator it = args.begin(); it != args.end(); it++)
	{
		xwstring arg = (*it);

		if(arg == _T("-i"))
			return InstallWin32Service(serviceName, versionInfo);

		if(arg == _T("-u"))
			return UninstallWin32Service(serviceName, versionInfo);
	}

	return RunAsConsoleApplication(serviceName, versionInfo);
}

