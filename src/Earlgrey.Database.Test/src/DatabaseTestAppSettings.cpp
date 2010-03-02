#include "stdafx.h"
#include "DatabaseTestAppSettings.h"

#include "Environment.h"
#include "Path.h"

namespace Earlgrey {

	DatabaseTestAppSettings::DatabaseTestAppSettings()
		: m_IniFile()
		, m_ConnectionString()
	{

	}

	DatabaseTestAppSettings::~DatabaseTestAppSettings()
	{

	}

	const _txstring& DatabaseTestAppSettings::IniFile() const
	{
		if(m_IniFile.empty() == false)
			return m_IniFile;

		_txstring baseDirectory( Environment::BaseDirectory() );
		m_IniFile = Path::Combine(baseDirectory, _T("Earlgrey.Database.Test.ini"));
		EARLGREY_ASSERT(m_IniFile.empty() == false);
		return m_IniFile;
	}

	const _txstring& DatabaseTestAppSettings::ConnectionString() const
	{
		if(m_ConnectionString.empty() == false)
			return m_ConnectionString;

		const int bufferSize = MAX_PATH;
		TCHAR buffer[bufferSize];

		const int length = ::GetPrivateProfileString(_T("database"), _T("ConnectionString"), NULL, buffer, bufferSize, IniFile().c_str());
		buffer[length] = NULL;

		m_ConnectionString = buffer;
		EARLGREY_ASSERT(m_ConnectionString.empty() == false);
		return m_ConnectionString;
	}

}
