#pragma once
#include "DefaultAppSettings.h"

class Win32ServiceSettings : public Earlgrey::DefaultAppSettings
{
public:
	virtual BOOL AllowOnlyOneInstance() const;
	virtual const TCHAR * const ShortName() const;

	virtual UnhandledExceptionCollectionPtr UnhandledExceptions();

private:
	std::tr1::shared_ptr<Earlgrey::UnhandledExceptionHandler> GetExceptionEMailer();
};