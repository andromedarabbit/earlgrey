#pragma once
#include "DefaultAppSettings.h"

class Win32ServiceSettings : public Earlgrey::DefaultAppSettings
{
public:
	virtual const TCHAR * const ShortName() const;
};