#include "stdafx.h"
#include "Registry.h"
#include "RegistryKey.h"

namespace Earlgrey
{

	RegistryKey Registry::ClassesRoot()
	{
		return RegistryKey(HKEY_CLASSES_ROOT);
	}

	RegistryKey Registry::CurrentConfig()
	{
		return RegistryKey(HKEY_CURRENT_CONFIG);
	}

	RegistryKey Registry::CurrentUser()
	{
		return RegistryKey(HKEY_CURRENT_USER);
	}

	RegistryKey Registry::LocalMachine()
	{
		return RegistryKey(HKEY_LOCAL_MACHINE);
	}

	RegistryKey Registry::Users()
	{
		return RegistryKey(HKEY_USERS);
	}

}