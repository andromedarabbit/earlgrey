#pragma once
// #include <Loki/Singleton.h>
#include "AppType.h"
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	
	class AppInfo : private Uncopyable
	{
	public: // Methods
		explicit AppInfo()
			: m_CurrentAppType(AppType::E_APPTYPE_NONE)
		{
		}

		BOOL InitInstance(AppType::E_Type appType);

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

	};
}
