#pragma once 
#include "Thread.h"

namespace Earlgrey
{
	class Server
	{
		void CreateServer();
	};

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
}