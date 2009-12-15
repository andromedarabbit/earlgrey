#include "stdafx.h"
#include "DefaultAppSettings.h"
#include "Environment.h"
#include "EarlgreyProcess.h"
#include "Path.h"

namespace Earlgrey
{
	DefaultAppSettings::DefaultAppSettings()
		: m_executableName(Process::MainModuleFileName())
		, m_shortNmae(Path::GetFileName(m_executableName))
		, m_versionInfo( FileVersionInfo::GetVersionInfo(m_executableName) )
	{
		EARLGREY_ASSERT(m_executableName.length() > 0);
	}

	DefaultAppSettings::~DefaultAppSettings()
	{

	}

	DWORD DefaultAppSettings::IOThreads() const
	{
		return Environment::ProcessorCount();
	}

	const TCHAR * const DefaultAppSettings::ShortName() const
	{
		return m_shortNmae.c_str();
	}

	const TCHAR * const DefaultAppSettings::LongName() const
	{
		return m_versionInfo.ProductName().c_str();
	}

	const TCHAR * const DefaultAppSettings::Description() const
	{	
		return m_versionInfo.FileDescription().c_str();
	}

	const TCHAR * const DefaultAppSettings::Version() const
	{	
		return m_versionInfo.ProductVersion().c_str();
	}


}