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
	}
	
	BOOL debugMode = (mode == SERVER_RUN_MODE_DEBUG);
	ServerService service(_T("EargreyServer"), _T("얼그레이 서버"), debugMode);

	if(debugMode)
	{
		service.OnStart(__argc, __wargv);
		return EXIT_SUCCESS;
	}

	if(mode == SERVER_RUN_MODE_INSTALL)
	{
		Win32ServiceInstaller installer(service);
		if(installer.InstallService() == FALSE)
			return EXIT_FAILURE;
		return EXIT_SUCCESS;
	}

	if(mode == SERVER_RUN_MODE_UNINSTALL)
	{
		Win32ServiceInstaller installer(service);
		if(installer.RemoveService() == FALSE)
			return EXIT_FAILURE;
		return EXIT_SUCCESS;
	}
		
	Win32Service::Run(service);
	return EXIT_SUCCESS;
}


/*
	using namespace TCLAP;

	bool debugMode = false;
	try
	{
		// tclap 가 유니코드를 지원하지 않으므로 ::GetCommandLineA()를 쓴다.
		CmdLine cmd("Command description message", ' ', Version(), false);

		StdOutput output;
		ServerHelpVisitor helpVisitor( &cmd, &output );

		SwitchArg helpArg("h","help", "Displays usage information and exits.", false, &helpVisitor);
		cmd.add(helpArg);

		SwitchArg debugArg("d","debug", "Start as a console application.", false);
		cmd.add(debugArg);

		cmd.parse(__argc, __argv);

		debugMode = debugArg.getValue();
	}
	catch ( ArgException& e )
	{ 
		std::cout << "ERROR: " << e.error() << " " << e.argId() << std::endl; 
		return EXIT_FAILURE;
	}
*/