#include "stdafx.h"
#include "WindowsRunnable.h"
#include "ServerService.h"


WindowsRunnable::WindowsRunnable(std::tr1::shared_ptr<ServerService> service) 
: m_service(service)
{
	EARLGREY_ASSERT(m_service != NULL);
}

WindowsRunnable::~WindowsRunnable() 
{
}

BOOL WindowsRunnable::Init()
{
	return TRUE;
}

DWORD WindowsRunnable::Run()
{
	/*MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{

	}*/
	HANDLE stopHandle = m_service->m_stopHandle;
	EARLGREY_ASSERT(stopHandle != NULL);

	while (::WaitForSingleObject(stopHandle, 10) != WAIT_OBJECT_0) {
		::Sleep(1000);
	}

	if( stopHandle )
	{
		EARLGREY_VERIFY(::CloseHandle(stopHandle));
	}

	return 0;
}

void WindowsRunnable::Stop()
{

}

void WindowsRunnable::Exit()
{

}