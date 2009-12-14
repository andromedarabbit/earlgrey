#pragma once
#include "AppType.h"
#include "EarlgreyAssert.h"
#include <vector>

namespace Earlgrey
{
	class Thread;
	class Uncopyable;
	class AppSettings;

	class Application : private Uncopyable
	{
		typedef std::vector<std::tr1::shared_ptr<Thread>> ThreadContainer;
	public: // Methods
		explicit Application(AppType::E_Type appType, const AppSettings& appSettings)
			: m_CurrentAppType(appType)
			, m_AppSettings(appSettings)
		{
		}

		~Application();
		
		BOOL InitInstance();

	private: // Methods
		inline AppType::E_Type CurrentAppType() const
		{
			EARLGREY_VERIFY(m_CurrentAppType > 0);
			return m_CurrentAppType;
		}

		//! �������α׷� �ν��Ͻ��� �̹� ���� ������ Ȯ���Ѵ�.
		BOOL CheckAppInstance(AppType::E_Type appType);


	private: // Fields
		AppType::E_Type m_CurrentAppType;
		const AppSettings& m_AppSettings;
		ThreadContainer m_IOThreads;
		ThreadContainer m_WaitThreads;
		ThreadContainer m_MainThreads;

	};
}
