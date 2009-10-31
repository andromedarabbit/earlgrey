// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ServerService.h"
#include "tclap/CmdLine.h"

// #include "tclap/SwitchArg.h"

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
	using namespace TCLAP;

	/*
	HRSRC hRsrc = ::FindResource(NULL, MAKEINTRESOURCE(VS_VERSION_INFO), RT_VERSION);
	if (hRsrc != NULL)
	{
		HGLOBAL hGlobal = ::LoadResource(NULL, hRsrc);
		if (hGlobal != NULL)
		{
			void* lpData = ::LockResource(hGlobal);

			// Get the info
			VS_FIXEDFILEINFO* pVerInfo = NULL;
			UINT uInfoSize;
			if (::VerQueryValue(lpData, _T("\\"), reinterpret_cast<LPVOID*>(&pVerInfo), &uInfoSize))
			{
				// use pVerInfo now
			}

			::FreeResource(hGlobal);
		}
	}

	*/

	try
	{
		// tclap 가 유니코드를 지원하지 않으므로 ::GetCommandLineA()를 쓴다.
		// CmdLine cmd(::GetCommandLineA());
		CmdLine cmd("Command description message", ' ', "0.9", true);

		DBG_UNREFERENCED_LOCAL_VARIABLE(cmd);
		// SwitchArg<string> debugSwitch("d", "debug", "Start as a console application", false, NULL);
		// cmd.add(debugSwitch)
		

		
	}
	catch ( ArgException& e )
	{ 
		std::cout << "ERROR: " << e.error() << " " << e.argId() << std::endl; 
	}

	// ::CommandLineToArgv(

	ServerService service(_T("EargreyServer"), _T("얼그레이 서버"));
	Win32Service::Run(service);

	return EXIT_SUCCESS;
}