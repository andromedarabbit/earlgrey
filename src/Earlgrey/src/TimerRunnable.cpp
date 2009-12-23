#include "stdafx.h"
#include "TimerRunnable.h"

namespace Earlgrey
{
	DWORD TimerRunnable::Run()
	{
		DateTime now(DateTime::Now());
		if( now < m_dueTime ) // ���� ù ���࿹��ð��� �� ����
			return EXIT_SUCCESS;

		if(m_period <= TimeSpan::Zero) // 1ȸ ����
			m_expired = TRUE;
		else if( now < (m_lastRunTime + m_period) )
			return EXIT_SUCCESS;

		m_callback(m_state);
		m_lastRunTime = now;

		return EXIT_SUCCESS;
	}

	TimerRunnable::IDType TimerRunnable::GenerateID()
	{
		return InterlockedIncrement(&TIMER_COUNT);
	}
}