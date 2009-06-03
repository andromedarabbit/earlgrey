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
		while( ProactorSingleton::Instance().HandleEvent() )
		{

		}
		return 0;
	}


	/**
	*/
	BOOL AcceptorRunnable::Init()
	{
		return TRUE;
	}

	DWORD AcceptorRunnable::Run()
	{
		while( AcceptProactorSingleton::Instance().HandleEvent() )
		{

		}
		return 0;
	}
}