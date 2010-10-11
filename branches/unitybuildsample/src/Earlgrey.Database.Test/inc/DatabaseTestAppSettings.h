#pragma once
#include "DefaultAppSettings.h"

#include <Loki/Singleton.h>
#include "NoLock.h"

namespace Earlgrey {

	class DatabaseTestAppSettings : public DefaultAppSettings
	{
		friend struct Loki::CreateUsingNew<DatabaseTestAppSettings>;

		explicit DatabaseTestAppSettings();
		virtual ~DatabaseTestAppSettings();

	public:
		const _txstring& IniFile() const;
		const _txstring& ConnectionString() const;

	private:
		mutable _txstring m_IniFile;
		mutable _txstring m_ConnectionString;
	};

	typedef
		Loki::SingletonHolder<DatabaseTestAppSettings, Loki::CreateUsingNew, Loki::NoDestroy,  Loki::SingleThreaded, NoLock> 
		DatabaseTestAppSettingsSingleton
		;

}
