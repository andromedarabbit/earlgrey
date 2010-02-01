#include "stdafx.h"
#include "Win32ServiceSettings.h"

#include "Win32ServiceHelper.h"

using namespace Earlgrey;

BOOL Win32ServiceSettings::AllowOnlyOneInstance() const
{
	if(Win32ServiceHelper::RunByWin32Service())
		return TRUE;

	return FALSE;
}

const TCHAR * const Win32ServiceSettings::ShortName() const
{
	return _T("EarlgreyServer");
}