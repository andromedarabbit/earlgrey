#pragma once
#include "Runnable.h"

namespace Earlgrey
{
	class ThreadRunnable : IRunnable
	{
		friend class Thread;
	public:
		explicit ThreadRunnable(); 
		virtual ~ThreadRunnable(); 

		DWORD Run();

	protected:
		virtual BOOL Init();
		virtual void Stop() = 0;
		virtual void Exit() = 0;

		virtual BOOL MeetsStopCondition() const = 0;
		virtual DWORD DoTask() = 0;
	};
}
