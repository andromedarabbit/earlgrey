#pragma once 
#include "ThreadRunnable.h"
#include "TimeSpan.h"

namespace Earlgrey
{
	class IOCPRunnable : public ThreadRunnable
	{
	public:
		explicit IOCPRunnable(); 
		virtual ~IOCPRunnable(); 

	protected:
		virtual BOOL Init();
		virtual void Stop();
		virtual void Exit();

		virtual BOOL MeetsStopCondition() const;
		virtual DWORD DoTask();

	private:
		BOOL m_isRunning;
		TimeSpan m_waitingTime;
	};

	
}