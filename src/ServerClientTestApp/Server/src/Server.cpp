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
// #include "ServerHelpVisitor.h"

// #include <tclap/CmdLine.h>
#include "tiostream.h"

#ifndef UNICODE
#error currently UNICODE should be defined!
#endif


using namespace std;
using namespace Earlgrey;

namespace 
{
	// using namespace std;
	/*
	std::string Version()
	{
		using namespace Earlgrey;

		// Getting a file version.
		_txstring executableName = Process::MainModuleFileName();
		FileVersionIngorfo versionInfo( FileVersionInfo::GetVersionInfo(executableName) );
		const _txstring fileVersion( versionInfo.FileVersion() );
		// TODO: numeric_cast에 버그가 있어서 임시로 코드를 바꾼다.
		// const int fileVersionLength = Math::numeric_cast<int>(fileVersion.length());
		const int fileVersionLength = static_cast<int>(fileVersion.length());

		// Convert the file verion to ANSI String
		char version[128];
		if( ::WideCharToMultiByte(::GetACP(), 0, fileVersion.c_str(), fileVersionLength + 1, version, sizeof(version), 0, 0) == 0)
		{ 
			// GetLastError();
			// ERROR_INSUFFICIENT_BUFFER 
			// ERROR_INVALID_FLAGS 
			// ERROR_INVALID_PARAMETER 
			throw std::exception("Converting Unicode string to ANSI failed!");
		}
		return std::string(version);
	}
	*/

	typedef xvector<xwstring>::Type ArgContainerType;

	void getArgs(ArgContainerType& args) {
		int argc = 0;
		wchar_t** argv = CommandLineToArgvW(GetCommandLineW(), &argc);
		// Earlgrey::xvector<std:wstring>::Type args;
		if (argv) {
			args.assign(argv, argv + argc);
			LocalFree(argv);
		}
		// return args;
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

	
	enum SERVER_RUN_MODE
	{
		SERVER_RUN_MODE_SERVICE
		, SERVER_RUN_MODE_DEBUG
		, SERVER_RUN_MODE_INSTALL
		, SERVER_RUN_MODE_UNINSTALL
	};


	SERVER_RUN_MODE mode = SERVER_RUN_MODE_SERVICE;

	ArgContainerType args;
	getArgs(args);

	for(ArgContainerType::iterator it = args.begin(); it != args.end(); it++)
	{
		xwstring arg = (*it);

		if(arg == _T("-d"))
			mode = SERVER_RUN_MODE_DEBUG;

		if(arg == _T("-i"))
			mode = SERVER_RUN_MODE_INSTALL;

		if(arg == _T("-u"))
			mode = SERVER_RUN_MODE_UNINSTALL;

		_tcout << arg << std::endl;
	}
	
	BOOL needsConsole = (
		mode == SERVER_RUN_MODE_DEBUG 
		|| mode == SERVER_RUN_MODE_INSTALL
		|| mode == SERVER_RUN_MODE_UNINSTALL
		);

	ServerService service(_T("EarlgreyServer"), _T("얼그레이 서버"), needsConsole);

	if(mode == SERVER_RUN_MODE_DEBUG)
	{
		service.OnStart(__argc, __wargv);
		_tcout << service.ServiceName() << _T(" ends.") << std::endl;
		return EXIT_SUCCESS;
	}

	if(mode == SERVER_RUN_MODE_SERVICE)
	{
		Win32Service::Run(service);
		return EXIT_SUCCESS;
	}

	Win32ServiceInstaller installer(service);
	installer.Description(_T("얼그레이 예제 서버입니다."));

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

	return EXIT_FAILURE;
}

