#include "stdafx.h"
#include "IOCP.h"
#include "Proactor.h"

namespace Earlgrey
{
	BOOL IOCPRunnable::Init()
	{
		return TRUE;
	}

	DWORD IOCPRunnable::Run()
	{
		while( ProactorSingleton::Instance().HandleEvent() )
		{

		}
	}
}