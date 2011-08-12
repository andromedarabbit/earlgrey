#pragma once
#include "AppSettings.h"
#include "txstring.h"
#include "FileVersionInfo.h"
#include "xlist.h"

namespace Earlgrey
{
	class DefaultAppSettings : public AppSettings
	{
	public:
		explicit DefaultAppSettings();
		virtual ~DefaultAppSettings();

		virtual BOOL AllowOnlyOneInstance() const;
		virtual DWORD NumberOfIOThreads() const;
		virtual DWORD NumberOfConcurrentIOThreads() const;
		virtual const TCHAR * const ShortName() const;
		virtual const TCHAR * const LongName() const;
		virtual const TCHAR * const Description() const;
		virtual const TCHAR * const Version() const;

		virtual UnhandledExceptionCollectionPtr UnhandledExceptions() const;
	
		// virtual ThreadIdType LoggingThreadId() const;
		virtual BOOL IsThisLoggingThread() const;
		virtual TimeSpan LoggingFlushInterval() const;

	private:
		_tstring m_executableName;
		_tstring m_shortName;
		FileVersionInfo m_versionInfo;
		UnhandledExceptionCollectionPtr m_UnhandledExceptions;
	};

}