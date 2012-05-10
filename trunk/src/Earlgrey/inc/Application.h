#pragma once
#include "EarlgreyAssert.h"
#include "Uncopyable.h"

#include <vector>
#include <functional>

namespace Earlgrey
{
	class Thread;
	class AppSettings;

	class Application : private Uncopyable
	{
		typedef std::vector<std::tr1::shared_ptr<Thread>> ThreadContainer;

	public: // Methods
		explicit Application(const AppSettings& appSettings);

		~Application();
		
		BOOL InitInstance();

	/*	const std::tr1::shared_ptr<Thread> MainThread() const
		{
			return m_MainThread;
		}*/

		// void Join();

		int Run();
		int Run(std::tr1::function<int()> mainFunc);

		int RunOnce(std::tr1::function<int()> mainFunc);

	private: // Methods
		inline const AppSettings & GetAppSettings() const
		{
			return m_AppSettings;
		}

		//! 응용프로그램 인스턴스가 이미 실행 중인지 확인한다.
		BOOL CheckAppInstance();
		BOOL CheckAppInstance(const WCHAR * appName);


	private: // Fields
		const AppSettings& m_AppSettings;
		ThreadContainer m_IOThreads;
		ThreadContainer m_WaitThreads;
		std::tr1::shared_ptr<Thread> m_MainThread;

	};
}
