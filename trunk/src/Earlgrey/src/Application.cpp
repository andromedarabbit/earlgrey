#include "stdafx.h"
#include "Application.h"

#include "AppSettings.h"
#include "SingleAppInstance.h"
#include "RuntimeCheck.h"
#include "ProcessInitializer.h"
#include "Acceptor.h"
#include "Environment.h" // Environment
#include "Thread.h" // Thread
#include "IOCP.h" // IOCPRunnable

#include "GlobalExceptionHandler.h"

namespace Earlgrey
{
	Application::Application(const AppSettings& appSettings)
		: m_AppSettings(appSettings)
		, m_IOThreads()
		, m_WaitThreads()
		, m_MainThreads()
	{
	}


	BOOL Application::InitInstance()
	{
		if( CheckAppInstance() == FALSE )
		{
			return FALSE;
		}

		GlobalExceptionHandler::Initialize();

		AppSettings::UnhandledExceptionCollectionPtr handlers = m_AppSettings.UnhandledExceptions();
		AppSettings::UnhandledExceptionCollection::const_iterator it = handlers->begin();
		for( ; it != handlers->end(); it++)
		{
			GlobalExceptionHandler::Register(*it);
		}
		
		// RuntimeCheck Ȱ��ȭ
		// \todo DoRtcTermination �� ���, ���� �θ���?
		DoRtcInitialization(); 

		// ���μ��� �켱����
		ProcessInitializer processInitializer;
		if( !processInitializer.Run() )
			return FALSE;

		// IOCP �ʱ�ȭ
		if( !ProactorSingleton::Instance().Initialize(m_AppSettings) )
			return FALSE;

		std::tr1::shared_ptr<Thread> mainThread = Thread::AttachThread("MainThread", WIN_MAIN_THREAD_ID);
		m_MainThreads.push_back(mainThread);

		// Create IO Thread
		// �ϴ� �����尡 ��ϵ��� �ʴ´ٰ� �����ϰ� ���μ��� ������ŭ �����带 �����Ѵ�. 
		const DWORD IOThreadCount = m_AppSettings.NumberOfIOThreads();
		if(IOThreadCount == 0)
			return TRUE;

		IO_THREAD_ID_END = IO_THREAD_ID_BEGIN + IOThreadCount - 1;
		EARLGREY_VERIFY(IO_THREAD_ID_BEGIN <= IO_THREAD_ID_END);
		EARLGREY_VERIFY(IO_THREAD_ID_END - IO_THREAD_ID_BEGIN + 1 <= MAX_IO_THREADS);

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

		std::tr1::shared_ptr<ThreadRunnable> acceptorThread(
			new AcceptorRunnable()
			);

		// Wait thread ID ���� �ʿ�
		std::tr1::shared_ptr<Thread> thread = Thread::CreateThread( acceptorThread, "AcceptorRunnable", 0 );
		m_WaitThreads.push_back(thread);

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
		GlobalExceptionHandler::UnregisterAll();
	}

	BOOL Application::CheckAppInstance()
	{
		return CheckAppInstance(m_AppSettings.ShortName());
	}

	BOOL Application::CheckAppInstance(const TCHAR * appName)
	{
		if( m_AppSettings.AllowOnlyOneInstance() == FALSE
			|| gSingleAppInstance::Instance().RegisterThisApp(appName) )
		{
			return TRUE;
		}

		_txstring msg(appName);
		msg += TEXT(" has been already started!");

		BOOL needMsgBox = TRUE; // TODO: �ӽ� �ڵ���
		if( needMsgBox )
			MessageBox(NULL, msg.c_str(), NULL, MB_OK);
		return FALSE;
	}


}
