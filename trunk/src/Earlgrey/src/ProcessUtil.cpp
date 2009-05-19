#include "stdafx.h"
#include "ProcessUtil.h"

namespace Earlgrey { namespace Utility {
	DWORD GetProcessorCount()
	{	
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		return sysinfo.dwNumberOfProcessors;	
	}
}}