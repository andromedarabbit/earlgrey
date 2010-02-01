#pragma once
#include "DefaultAppSettings.h"

class Win32ServiceSettings : public Earlgrey::DefaultAppSettings
{
public:
	virtual BOOL AllowOnlyOneInstance() const;
	virtual const TCHAR * const ShortName() const;
};