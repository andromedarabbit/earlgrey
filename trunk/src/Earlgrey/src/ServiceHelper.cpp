#include "stdafx.h"
#include "ServiceHelper.h"
#include "EarlgreyProcess.h"
#include "StringComparison.hpp"

namespace Earlgrey
{
	namespace ServiceProcess
	{
		namespace Helper
		{
			BOOL RunByWin32Service()
			{
				const _tstring parentProcessName(Process::GetParentProcessName(GetCurrentProcessId()));

				StringComparison<STRCMP_CURRENT_CULTURE_IGNORECASE> stringComparer;
				if(
					stringComparer.Equals(_T("services.exe"), parentProcessName.c_str())
					)
				{
					return TRUE;
				}
				return FALSE;
			}

		}
	}
}