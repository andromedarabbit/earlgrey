#include "stdafx.h"

#include "ServerInit.h"

#include "Environment.h"

namespace Earlgrey
{
	// fixme
	//void Init()
	//{
	//	// Initialize IOCP
	//	ProactorSingleton::Instance().Initialize();

	//	// Initialize Socket 
	//	SocketSubsystem::InitializeSubSystem();

	//	// Create IO Thread
	//	DWORD IOThreadCount = Environment::ProcessorCount();
	//	//EARLGREY_ASSERT(IOThreadCount < MAX_IO_THREAD_COUNT);
	//	EARLGREY_ASSERT(IOThreadCount > 0);
	//	for (DWORD i = 0; i < IOThreadCount; i++)
	//	{
	//		std::tr1::shared_ptr<Thread> thread = Thread::CreateRunningThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new IOCPRunnable())), "IOCPRunnable" );
	//		thread->SetProcessorAffinity(i, IOThreadCount);
	//		thread->SetPriority(THREAD_PRIORITY_HIGHEST);			
	//	}

	//}

	void ServerCreate(DWORD /*Port*/)
	{
		/*Acceptor* acceptor = new Acceptor(Port);
		acceptor->Initialize();*/
	}

	void ClientCreate(DWORD /*Port*/)
	{
		//Connector* connector = new Connector();
		//char* ServerIP = "localhost";//! TODO : type and ip
		//connector->Connect(ServerIP, Port);
	}

	void ServerCreated()
	{
		// Create Acceptor Thread
		// \todo 스레드 처리
		/*std::tr1::shared_ptr<IRunnable> acceptorThread (
			new AcceptorRunnable()
			);
		std::tr1::shared_ptr<Thread> thread = Thread::CreateRunningThread( acceptorThread, "AcceptorRunnable" );
		DBG_UNREFERENCED_LOCAL_VARIABLE(thread);*/

	}
}