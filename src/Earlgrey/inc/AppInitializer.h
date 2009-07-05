#pragma once
#include <vector>

// #include <Loki/Singleton.h>
#include "AppType.h"
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	class Thread;
	
	class AppInfo : private Uncopyable
	{
		typedef std::vector<Thread*> ThreadContainer;
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

	};
}
