#include "stdafx.h"

#include "ServerInit.h"
#include "IOCP.h"

namespace Earlgrey
{
	/**
	*/
	BOOL IOCPRunnable::Init()
	{
		return TRUE;
	}

	DWORD IOCPRunnable::Run()
	{
		const TimeValueType WaitTime = 10; // msec
		while( IsRunning)
		{
			ProactorSingleton::Instance().HandleEvent(WaitTime);

		}
		return 0;
	}
}