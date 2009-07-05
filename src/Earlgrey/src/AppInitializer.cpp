#include "stdafx.h"
#include "AppInitializer.h"
#include "SingleInstance.h"
#include "StlCustom.h" 
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
		// Initialize IOCP
		if( !ProactorSingleton::Instance().Initialize() )
			return FALSE;

		if( !SocketSubsystem::InitializeSubSystem() )
			return FALSE;

		// Create IO Thread
		DWORD IOThreadCount = Environment::ProcessorCount();
		//EARLGREY_ASSERT(IOThreadCount < MAX_IO_THREAD_COUNT);
		for (DWORD i = 0; i < IOThreadCount; i++)
		{
			Thread* thread = Thread::CreateRunningThread( std::tr1::shared_ptr<IRunnable>(static_cast<IRunnable*>(new IOCPRunnable())), "IOCPRunnable" );
			thread->SetProcessorAffinity(i, IOThreadCount);
			thread->SetPriority(THREAD_PRIORITY_HIGHEST);			
		}




		return TRUE;
	}


	BOOL AppInfo::CheckAppInstance(AppType::E_Type appType)
	{
		if( gSingleInstance::Instance().IsRunning(appType) )
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
