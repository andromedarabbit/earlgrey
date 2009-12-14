#include "stdafx.h"
#include "Application.h"

#include "AppSettings.h"
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

	BOOL Application::InitInstance()
	{
		if( !CheckAppInstance(m_CurrentAppType) )
			return FALSE;

		// RuntimeCheck 활성화
		// \todo DoRtcTermination 은 어디서, 언제 부르나?
		DoRtcInitialization(); 

		// 프로세스 우선순위
		ProcessInitializer processInitializer;
		if( !processInitializer.Run() )
			return FALSE;

		// 네트워크 초기화
		if( !SocketSubsystem::InitializeSubSystem() )
			return FALSE;

		// IOCP 초기화
		if( !ProactorSingleton::Instance().Initialize() )
			return FALSE;

		std::tr1::shared_ptr<Thread> mainThread = Thread::AttachThread("MainThread", WIN_MAIN_THREAD_ID);
		m_MainThreads.push_back(mainThread);

		// Create IO Thread
		// 일단 스레드가 블록되지 않는다고 가정하고 프로세스 개수만큼 스레드를 생성한다. 
		const DWORD IOThreadCount = m_AppSettings.IOThreads();
		IO_THREAD_ID_END = IO_THREAD_ID_BEGIN + IOThreadCount - 1;

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

		/*for (DWORD i = 0; i < 1; i++) // FIXME: wait thread 갯수 지정하는거 필요
		{

			std::tr1::shared_ptr<IRunnable> acceptorThread (
				new AcceptorRunnable()
				);
			std::tr1::shared_ptr<Thread> thread = Thread::CreateThread( acceptorThread, "AcceptorRunnable" );
			m_WaitThreads.push_back(thread);

		}*/

		return TRUE;
	}

	Application::~Application()
	{
		for (ThreadContainer::const_iterator i = m_IOThreads.begin();  i != m_IOThreads.end(); i++ ){
			std::tr1::shared_ptr<Thread> thread = *i;

			thread->Stop();
			thread->Join();
		}
		m_IOThreads.clear();
	}

	BOOL Application::CheckAppInstance(AppType::E_Type appType)
	{
		if( gSingleInstance::Instance().IsRunning(appType) == FALSE )
		{
			this->m_CurrentAppType = appType;
			return TRUE;
		}

		BOOL needMsgBox = TRUE; // TODO: 임시 코드임

		_txstring msg = _txstring(AppType::Names[appType]) + TEXT(" has been already started!");
		if( needMsgBox )
			MessageBox(NULL, msg.c_str(), NULL, MB_OK);
		return FALSE;
	}


}
