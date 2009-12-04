#include "stdafx.h"
#include "Win32ServiceRunnable.h"
#include "ServerService.h"


Win32ServiceRunnable::Win32ServiceRunnable(std::tr1::shared_ptr<ServerService> service) 
	: m_service(service)
{
	EARLGREY_ASSERT(m_service != NULL);
}

Win32ServiceRunnable::~Win32ServiceRunnable() 
{
}

BOOL Win32ServiceRunnable::Init()
{
	return TRUE;
}

DWORD Win32ServiceRunnable::Run()
{
	HANDLE stopHandle = m_service->m_stopHandle;
	EARLGREY_ASSERT(stopHandle != NULL);

	while (::WaitForSingleObject(stopHandle, 10) != WAIT_OBJECT_0) 
	{
		::Sleep(1000);
	}

	if( stopHandle != NULL )
	{
		EARLGREY_VERIFY(::CloseHandle(stopHandle));
	}

	return 0;
}

void Win32ServiceRunnable::Stop()
{

}

void Win32ServiceRunnable::Exit()
{

}