#include "stdafx.h"
#include "Win32ServiceRunnable.h"
#include "ServerService.h"
#include "Thread.h"

using namespace Earlgrey;

Win32ServiceRunnable::Win32ServiceRunnable(HANDLE stopHandle) 
	: m_stopHandle(stopHandle)
{
	EARLGREY_ASSERT(m_stopHandle != NULL);
}

Win32ServiceRunnable::~Win32ServiceRunnable() 
{
}

BOOL Win32ServiceRunnable::Init()
{
	EARLGREY_ASSERT( Thread::CurrentThread()->ThreadId() == WIN_MAIN_THREAD_ID );
	return TRUE;
}

BOOL Win32ServiceRunnable::MeetsStopCondition() const
{
	EARLGREY_ASSERT(m_stopHandle != NULL);
	return ::WaitForSingleObject(m_stopHandle, 10) == WAIT_OBJECT_0;
}

DWORD Win32ServiceRunnable::DoTask()
{
	EARLGREY_ASSERT(m_stopHandle != NULL);
	::Sleep(10);
	return EXIT_SUCCESS;
}

void Win32ServiceRunnable::Stop()
{
	EARLGREY_VERIFY(::SetEvent(m_stopHandle));
}

void Win32ServiceRunnable::Exit()
{
	if( m_stopHandle != NULL )
	{
		EARLGREY_VERIFY(::CloseHandle(m_stopHandle));
	}
}