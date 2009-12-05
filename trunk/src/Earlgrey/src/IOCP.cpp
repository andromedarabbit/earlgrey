#include "stdafx.h"

#include "ServerInit.h"
#include "IOCP.h"

namespace Earlgrey
{
	IOCPRunnable::IOCPRunnable() 
		: m_isRunning(TRUE) 
	{
	}

	IOCPRunnable::~IOCPRunnable()
	{

	}

	BOOL IOCPRunnable::Init()
	{
		return TRUE;
	}

	DWORD IOCPRunnable::Run()
	{
		TimeSpan WaitTime(TimeSpan::FromMilliseconds(10));
		while( m_isRunning)
		{
			ProactorSingleton::Instance().HandleEvent(WaitTime);

		}
		return 0;
	}

	void IOCPRunnable::Stop() 
	{ 
		m_isRunning = FALSE; 
	}

	void IOCPRunnable::Exit()
	{

	}

}