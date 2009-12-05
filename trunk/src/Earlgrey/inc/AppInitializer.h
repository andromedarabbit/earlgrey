#pragma once
#include "AppType.h"
#include "EarlgreyAssert.h"

#include <vector>

namespace Earlgrey
{
	class Thread;
	class Uncopyable;

	class AppInfo : private Uncopyable
	{
		typedef std::vector<std::tr1::shared_ptr<Thread>> ThreadContainer;
	public: // Methods
		explicit AppInfo()
			: m_CurrentAppType(AppType::E_APPTYPE_NONE)
		{
		}

		~AppInfo();
		
		BOOL InitInstance(AppType::E_Type appType);

	private: // Methods
		inline AppType::E_Type CurrentAppType() const
		{
			EARLGREY_VERIFY(m_CurrentAppType > 0);
			return m_CurrentAppType;
		}

		//! 응용프로그램 인스턴스가 이미 실행 중인지 확인한다.
		BOOL CheckAppInstance(AppType::E_Type appType);


	private: // Fields
		AppType::E_Type m_CurrentAppType;
		ThreadContainer m_IOThreads;
		ThreadContainer m_WaitThreads;
		ThreadContainer m_MainThreads;

	};
}
