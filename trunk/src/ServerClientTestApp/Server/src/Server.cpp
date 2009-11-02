// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerService.h"
#include "txstring.h"
#include "EarlgreyProcess.h"
#include "FileVersionInfo.h"
#include "EarlgreyMath.h"
#include "ServerHelpVisitor.h"

// #include <tclap/CmdLine.h>


#ifndef UNICODE
#error currently UNICODE should be defined!
#endif

namespace 
{
	std::string Version()
	{
		using namespace Earlgrey;

		// Getting a file version.
		_txstring executableName = Process::MainModuleFileName();
		FileVersionInfo versionInfo( FileVersionInfo::GetVersionInfo(executableName) );
		const _txstring fileVersion( versionInfo.FileVersion() );
		// TODO: numeric_cast�� ���װ� �־ �ӽ÷� �ڵ带 �ٲ۴�.
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

	using namespace std;
	using namespace Earlgrey;
	/*
	using namespace TCLAP;

	bool debugMode = false;
	try
	{
		// tclap �� �����ڵ带 �������� �����Ƿ� ::GetCommandLineA()�� ����.
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


	// �ӽ÷� �ܼ� ���� ����.
	bool debugMode = true;	
	ServerService service(_T("EargreyServer"), _T("��׷��� ����"), debugMode);

	if(debugMode)
		service.OnStart(__argc, __wargv);
	else
		Win32Service::Run(service);

	return EXIT_SUCCESS;
}