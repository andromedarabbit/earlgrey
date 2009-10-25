#pragma once
#include <Loki/Singleton.h>
#include "AppType.h"

namespace Earlgrey
{
	class SingleAppInstance
	{
		friend struct Loki::CreateUsingNew<SingleAppInstance>;

	private: // Fields
		HANDLE m_Mutex;
		BOOL m_ExecutedOnce;

	private:
		explicit SingleAppInstance()
			: m_Mutex(NULL)
			, m_ExecutedOnce(FALSE)
		{
		}

		~SingleAppInstance()
		{
			if(m_Mutex != NULL)
			{
				ReleaseMutex(m_Mutex);
			}
		}

	public:
		BOOL IsRunning(AppType::E_Type appType);
	};


	typedef 
		Loki::SingletonHolder<SingleAppInstance, Loki::CreateUsingNew, Loki::DefaultLifetime, Loki::SingleThreaded> 
		gSingleInstance;
}