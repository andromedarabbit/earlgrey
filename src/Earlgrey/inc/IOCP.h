#pragma once 

namespace Earlgrey
{
	class IOCPRunnable : public IRunnable
	{
	public:
		explicit IOCPRunnable() {}
		virtual ~IOCPRunnable() {}

		virtual BOOL Init();
		virtual DWORD Run();
		virtual void Stop() {}
		virtual void Exit() {}
	};

	class AcceptorRunnable : public IRunnable
	{
	public:
		explicit AcceptorRunnable() {}
		virtual ~AcceptorRunnable() {}

		virtual BOOL Init();
		virtual DWORD Run();
		virtual void Stop() {}
		virtual void Exit() {}
	};
}