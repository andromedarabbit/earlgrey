#pragma once
#include <Loki/Singleton.h>
#include "AppType.h"
#include "xmap.h"
#include "txstring.h"

namespace Earlgrey
{
	//! \ref http://www.visualcpp.org/?p=74
	//! \ref http://www.ai.uga.edu/mc/SingleInstance.html
	//! \ref http://www.ubercode.com/write-programs-for-windows-api.html
	class SingleAppInstance
	{
		friend struct Loki::CreateUsingNew<SingleAppInstance>;

	private:
		explicit SingleAppInstance()
		{
		}

		~SingleAppInstance();
		

	public:
		BOOL IsRunning(const TCHAR * appName);
		BOOL IsRunning(AppType::E_Type appType);


	private:
		typedef xmap<_txstring, HANDLE>::Type MutexCollectionType;
		MutexCollectionType m_handles;
	};


	typedef 
		Loki::SingletonHolder<SingleAppInstance, Loki::CreateUsingNew, Loki::DefaultLifetime, Loki::SingleThreaded> 
		gSingleInstance;
}