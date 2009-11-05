#pragma once 

namespace Earlgrey
{
	class IOCPRunnable : public IRunnable
	{
	public:
		IOCPRunnable() {}
		virtual ~IOCPRunnable() {}

		virtual BOOL Init();
		virtual DWORD Run();
		virtual void Stop() {}
		virtual void Exit() {}

	private:
		inline void DoRegularTask();
	};
}