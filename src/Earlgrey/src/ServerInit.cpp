#include "stdafx.h"
#include "ServerInit.h"

#include "Thread.h"

#include "Socket.h"
#include "Proactor.h"
#include "Acceptor.h"
#include "IOCP.h"
#include "Connector.h"

namespace Earlgrey
{
	void ServerInit()
	{
		// Initialize IOCP
		ProactorSingleton::Instance().Initialize();

		// Initialize Socket 
		SocketSubsystem::InitializeSubSystem();

		// Create IO Thread
		DWORD IOThreadCount = Thread::GetProcessorCount();
		//EARLGREY_ASSERT(IOThreadCount < MAX_IO_THREAD_COUNT);
		for (DWORD i = 0; i < IOThreadCount; i++)
		{
			Thread* thread = Thread::CreateRunningThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new IOCPRunnable())), "IOCPRunnable" );
			thread->SetProcessorAffinity(i, IOThreadCount);
			thread->SetPriority(THREAD_PRIORITY_HIGHEST);			
		}

	}

	void ServerCreate(DWORD Port)
	{
		Acceptor* acceptor = new Acceptor(Port);
		acceptor->Initialize();
	}

	void ClientCreate(DWORD Port)
	{
		Connector* connector = new Connector();
		char* ServerIP = "localhost";//! TODO : type and ip
		connector->Connect(ServerIP, Port);
	}

	void ServerCreated()
	{
		// Create Acceptor Thread
		Thread::CreateRunningThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new AcceptorRunnable())), "AcceptorRunnable" );
	}
}