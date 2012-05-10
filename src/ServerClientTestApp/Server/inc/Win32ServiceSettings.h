#pragma once
#include "ServiceSettings.h"

class Win32ServiceSettings : public Earlgrey::ServiceProcess::ServiceSettings
{
public:
	virtual const WCHAR * const ShortName() const;

	virtual UnhandledExceptionCollectionPtr UnhandledExceptions() const;

private:
	std::tr1::shared_ptr<Earlgrey::UnhandledExceptionHandler> GetExceptionEMailer() const;
};