#pragma once
// #include <Loki/Singleton.h>
#include <AppType.h>
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	
	class AppInitializer : private Uncopyable
	{
	public: // Methods
		explicit AppInitializer()
			: m_CurrentAppType(
			static_cast<AppType::E_Type>(0)
			)
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
