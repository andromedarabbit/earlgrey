#include "stdafx.h"
#include "TimerRunnable.h"

namespace Earlgrey
{
	DWORD TimerRunnable::Run()
	{
		DateTime now(DateTime::Now());
		if( now < m_dueTime ) // 아직 첫 실행예약시각이 안 됐음
			return EXIT_SUCCESS;

		if(m_period <= TimeSpan::Zero) // 1회 실행
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