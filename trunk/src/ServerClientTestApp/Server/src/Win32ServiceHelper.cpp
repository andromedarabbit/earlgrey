#include "stdafx.h"
#include "Win32ServiceHelper.h"
#include "EarlgreyProcess.h"
#include "StringComparison.hpp"

BOOL Win32ServiceHelper::RunByWin32Service()
{
	using namespace Earlgrey;

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