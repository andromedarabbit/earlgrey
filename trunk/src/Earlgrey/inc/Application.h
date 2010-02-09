#pragma once
#include "EarlgreyAssert.h"
#include "Uncopyable.h"
#include <vector>

namespace Earlgrey
{
	class Thread;
	class AppSettings;

	class Application : private Uncopyable
	{
		typedef std::vector<std::tr1::shared_ptr<Thread>> ThreadContainer;
	public: // Methods
		explicit Application(AppSettings& appSettings)
			: m_AppSettings(appSettings)
		{
		}

		~Application();
		
		BOOL InitInstance();

	private: // Methods
		inline const AppSettings & GetAppSettings() const
		{
			return m_AppSettings;
		}

		//! �������α׷� �ν��Ͻ��� �̹� ���� ������ Ȯ���Ѵ�.
		BOOL CheckAppInstance();
		BOOL CheckAppInstance(const TCHAR * appName);


	private: // Fields
		AppSettings& m_AppSettings;
		ThreadContainer m_IOThreads;
		ThreadContainer m_WaitThreads;
		ThreadContainer m_MainThreads;

	};
}