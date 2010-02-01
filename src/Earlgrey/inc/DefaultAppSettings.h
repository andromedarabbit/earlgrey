#pragma once
#include "AppSettings.h"
#include "txstring.h"
#include "FileVersionInfo.h"

namespace Earlgrey
{
	class FileVersionInfo;

	class DefaultAppSettings : public AppSettings
	{
	public:
		explicit DefaultAppSettings();
		virtual ~DefaultAppSettings();

		virtual BOOL AllowOnlyOneInstance() const;
		virtual DWORD IOThreads() const;
		virtual const TCHAR * const ShortName() const;
		virtual const TCHAR * const LongName() const;
		virtual const TCHAR * const Description() const;
		virtual const TCHAR * const Version() const;

	private:
		_tstring m_executableName;
		_tstring m_shortNmae;
		FileVersionInfo m_versionInfo;
	};

}