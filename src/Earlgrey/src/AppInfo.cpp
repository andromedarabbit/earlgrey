#include "stdafx.h"
#include "AppInfo.h"
#include "SingleInstance.h"

namespace Earlgrey
{
	BOOL AppInfo::CheckAppInstance(AppType::E_Type appType)
	{
		return gSingleInstance::Instance().IsRunning(appType);	
	}


}
