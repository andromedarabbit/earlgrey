#include "stdafx.h"
#include "Win32ServiceRunnable.h"
#include "Thread.h"

using namespace Earlgrey;

Win32ServiceRunnable::Win32ServiceRunnable() 
	: m_meetsStopCondition(FALSE)
{

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
	return m_meetsStopCondition;
}

DWORD Win32ServiceRunnable::DoTask()
{
	::Sleep(10);
	return EXIT_SUCCESS;
}

void Win32ServiceRunnable::Stop()
{
	m_meetsStopCondition = TRUE;
}

void Win32ServiceRunnable::Exit()
{
}