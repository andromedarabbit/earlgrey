#include "stdafx.h"
#include "ServerService.h"

#include "tiostream.h"
// #include "ServerInit.h"
#include "ServerConnection.h"
#include "WindowsRunnable.h"
#include "AppInitializer.h"

using namespace Earlgrey;


ServerService::ServerService(
   const TCHAR * serviceName
   , const TCHAR * displayName 
   )
   : Win32Service(serviceName, displayName)
   , m_console()
{
	EARLGREY_VERIFY(m_console.Open());
	m_console.WindowTitle(_T("Earlgrey Server"));

	m_console.WriteLine(_T("Console::WriteLine() works!"));

	m_console.RedirectStdIO();
	_tcout << _T("Server started!") << std::endl;

}

ServerService::~ServerService()
{

}


void ServerService::OnStart(DWORD argc, LPTSTR * argv)
{
	AppInfo app;
	app.InitInstance(AppType::E_APPTYPE_DEFAULT);

	//ServerCreate(100);
	Acceptor<ServerConnection>* acceptor = new Acceptor<ServerConnection>(100);
	acceptor->Initialize();

	std::tr1::shared_ptr<Thread> serverThread = Thread::CreateRunningThread( 
		std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new WindowsRunnable())), "WindowsRunnable" );

	serverThread->Join();

}

void ServerService::OnStop()
{

}