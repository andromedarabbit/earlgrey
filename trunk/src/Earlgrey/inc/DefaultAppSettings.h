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
		virtual const WCHAR * const ShortName() const;
		virtual const WCHAR * const LongName() const;
		virtual const WCHAR * const Description() const;
		virtual const WCHAR * const Version() const;

		virtual UnhandledExceptionCollectionPtr UnhandledExceptions() const;
	
		// virtual ThreadIdType LoggingThreadId() const;
		virtual BOOL IsThisLoggingThread() const;
		virtual TimeSpan LoggingFlushInterval() const;

	private:
		std::wstring m_executableName;
		std::wstring m_shortName;
		FileVersionInfo m_versionInfo;
		UnhandledExceptionCollectionPtr m_UnhandledExceptions;
	};

}