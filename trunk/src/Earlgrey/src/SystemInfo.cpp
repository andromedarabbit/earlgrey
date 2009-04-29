#include "stdafx.h"
#include "SystemInfo.h"
#include "Path.h"
#include <psapi.h>


namespace Earlgrey
{
	_txstring SystemInfo::BaseDirectory()
	{
		TCHAR modName[MAX_PATH];
		if(GetModuleFileNameEx(GetCurrentProcess(), NULL, modName, _countof(modName)) == 0)
		{
			// \todo ���� ó�� - GetLastError

		}

		_txstring directory = modName;
		return Path::GetDirectoryName(directory);
	}

}