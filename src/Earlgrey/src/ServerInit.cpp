#include "stdafx.h"
#include "ServerInit.h"

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

		// Create Acceptor Thread
		Thread::CreateRunningThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new AcceptorRunnable())), "AcceptorRunnable" );

	}

	void ServerCreate(DWORD Port)
	{
		Acceptor* GAcceptor = new Acceptor(Port);
		GAcceptor->Initialize();
	}
}