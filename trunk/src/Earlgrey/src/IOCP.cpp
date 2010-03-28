#include "stdafx.h"
#include "IOCP.h"
#include "Thread.h"

namespace Earlgrey
{
	IOCPRunnable::IOCPRunnable() 
		: m_isRunning(TRUE) 
		, m_waitingTime(TimeSpan::FromMilliseconds(10))
	{
	}

	IOCPRunnable::~IOCPRunnable()
	{

	}

	BOOL IOCPRunnable::Init()
	{
		EARLGREY_ASSERT( IsValidIOThreadId(Thread::CurrentThread()->ThreadId()) );
		return TRUE;
	}

	BOOL IOCPRunnable::MeetsStopCondition() const
	{
		return !m_isRunning;
	}

	DWORD IOCPRunnable::DoTask()
	{
		ProactorSingleton::Instance().HandleEvent(m_waitingTime);
		return EXIT_SUCCESS;
	}

	void IOCPRunnable::Stop() 
	{ 
		m_isRunning = FALSE; 
	}

	void IOCPRunnable::Exit()
	{

	}
}