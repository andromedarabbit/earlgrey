#include "stdafx.h"
#include "AppInitializer.h"
#include "SingleAppInstance.h"
#include "RuntimeCheck.h"
#include "ProcessInitializer.h"
#include "Socket.h"
#include "Proactor.h" // ProactorSingleton
#include "Environment.h" // Environment
#include "Thread.h" // Thread
#include "IOCP.h" // IOCPRunnable

namespace Earlgrey
{

	BOOL AppInfo::InitInstance(AppType::E_Type appType)
	{
		if( !CheckAppInstance(appType) )
			return FALSE;

		// RuntimeCheck Ȱ��ȭ
		// \todo DoRtcTermination �� ���, ���� �θ���?
		DoRtcInitialization(); 

		// ���μ��� �켱����
		ProcessInitializer processInitializer;
		if( !processInitializer.Run() )
			return FALSE;

		// ��Ʈ��ũ �ʱ�ȭ
		if( !SocketSubsystem::InitializeSubSystem() )
			return FALSE;

		// IOCP �ʱ�ȭ
		if( !ProactorSingleton::Instance().Initialize() )
			return FALSE;

		std::tr1::shared_ptr<Thread> mainThread = Thread::AttachThread("MainThread", WIN_MAIN_THREAD_ID);
		m_MainThreads.push_back(mainThread);

		// Create IO Thread
		// �ϴ� �����尡 ��ϵ��� �ʴ´ٰ� �����ϰ� ���μ��� ������ŭ �����带 �����Ѵ�. 
		DWORD IOThreadCount = Environment::ProcessorCount();
		for (DWORD i = 0; i < IOThreadCount; i++)
		{
			std::tr1::shared_ptr<Thread> thread = Thread::CreateThread( 
				std::tr1::shared_ptr<ThreadRunnable>(static_cast<ThreadRunnable*>(new IOCPRunnable()))
				, "IOCPRunnable" 
				, i + 1
				);
			EARLGREY_ASSERT(thread != NULL);
			m_IOThreads.push_back(thread);

			thread->SetProcessorAffinity(i, IOThreadCount);
			thread->SetPriority(THREAD_PRIORITY_HIGHEST);			
		}

		/*for (DWORD i = 0; i < 1; i++) // FIXME: wait thread ���� �����ϴ°� �ʿ�
		{

			std::tr1::shared_ptr<IRunnable> acceptorThread (
				new AcceptorRunnable()
				);
			std::tr1::shared_ptr<Thread> thread = Thread::CreateThread( acceptorThread, "AcceptorRunnable" );
			m_WaitThreads.push_back(thread);

		}*/

		return TRUE;
	}

	AppInfo::~AppInfo()
	{
		for (ThreadContainer::const_iterator i = m_IOThreads.begin();  i != m_IOThreads.end(); i++ ){
			std::tr1::shared_ptr<Thread> thread = *i;

			thread->Stop();
			thread->Join();
		}
		m_IOThreads.clear();
	}

	BOOL AppInfo::CheckAppInstance(AppType::E_Type appType)
	{
		if( gSingleInstance::Instance().IsRunning(appType) == FALSE )
		{
			this->m_CurrentAppType = appType;
			return TRUE;
		}

		BOOL needMsgBox = TRUE; // TODO: �ӽ� �ڵ���

		_txstring msg = _txstring(AppType::Names[appType]) + TEXT(" has been already started!");
		if( needMsgBox )
			MessageBox(NULL, msg.c_str(), NULL, MB_OK);
		return FALSE;
	}


}
