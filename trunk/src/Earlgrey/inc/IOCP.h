#pragma once 

namespace Earlgrey
{
	class IOCPRunnable : public IRunnable
	{
	public:
		IOCPRunnable() : IsRunning(TRUE) {}
		virtual ~IOCPRunnable() {}

		virtual BOOL Init();
		virtual DWORD Run();
		virtual void Stop() { IsRunning = FALSE; }
		virtual void Exit() {}

	private:
		BOOL IsRunning;
		inline void DoRegularTask();
	};
}