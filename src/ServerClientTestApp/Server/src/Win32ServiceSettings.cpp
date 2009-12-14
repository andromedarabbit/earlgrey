#include "stdafx.h"
#include "Win32ServiceSettings.h"

using namespace Earlgrey;

const TCHAR * const Win32ServiceSettings::ShortName() const
{
	return _T("EarlgreyServer");
}