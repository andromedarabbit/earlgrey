#include "stdafx.h"
#include "Win32ServiceSettings.h"

using namespace Earlgrey;

BOOL Win32ServiceSettings::AllowOnlyOneInstance() const
{
	return FALSE;
}

const TCHAR * const Win32ServiceSettings::ShortName() const
{
	return _T("EarlgreyServer");
}