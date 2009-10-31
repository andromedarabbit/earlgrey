// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
// #include "ServerInit.h"
// #include "AppInitializer.h"

// #include "EarlgreyAssert.h"
// #include "Console.h"
// #include "tiostream.h"
#include "ServerService.h"

using namespace Earlgrey;


/*
int _tmain(int argc, _TCHAR* argv[])
{
	AppInfo app;
	app.InitInstance(AppType::E_APPTYPE_DEFAULT);

	//ServerCreate(100);
	Acceptor<ServerConnection>* acceptor = new Acceptor<ServerConnection>(100);
	acceptor->Initialize();

	std::tr1::shared_ptr<Thread> serverThread = Thread::CreateRunningThread( 
		std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new WindowsRunnable())), "WindowsRunnable" );

	serverThread->Join();
	
	return EXIT_SUCCESS;
}
*/
// #include "..\..\..\vendor\argstream\\argstream.h"
#include "tclap/CmdLine.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	using namespace TCLAP;

	try
	{
		// tclap 가 유니코드를 지원하지 않으므로 ::GetCommandLineA()를 쓴다.
		CmdLine cmd(::GetCommandLineA());
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