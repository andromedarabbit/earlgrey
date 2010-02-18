#pragma once
#include "ServiceSettings.h"

class Win32ServiceSettings : public Earlgrey::ServiceProcess::ServiceSettings
{
public:
	virtual const TCHAR * const ShortName() const;

	virtual UnhandledExceptionCollectionPtr UnhandledExceptions();

private:
	std::tr1::shared_ptr<Earlgrey::UnhandledExceptionHandler> GetExceptionEMailer();
};