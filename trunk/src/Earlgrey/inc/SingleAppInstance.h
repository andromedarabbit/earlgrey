#pragma once
#include <Loki/Singleton.h>
#include "tstring.h"

namespace Earlgrey
{
	//! \ref http://www.visualcpp.org/?p=74
	//! \ref http://www.ai.uga.edu/mc/SingleInstance.html
	//! \ref http://www.ubercode.com/write-programs-for-windows-api.html
	class SingleAppInstance
	{
		friend struct Loki::CreateUsingNew<SingleAppInstance>;

	private:
		explicit SingleAppInstance();
		~SingleAppInstance();
		
	public:
		BOOL RegisterThisApp(const WCHAR * const appName);

	private:
		std::wstring m_thisAppName;
		HANDLE m_thisAppMutex;
	};


	typedef 
		Loki::SingletonHolder<SingleAppInstance, Loki::CreateUsingNew, Loki::DefaultLifetime, Loki::SingleThreaded> 
		gSingleAppInstance;
}