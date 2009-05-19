#include "stdafx.h"
#include "Environment.h"
#include "Path.h"
#include <Psapi.h> // GetModuleFileNameEx

namespace Earlgrey 
{ 
	const TCHAR* const Environment::NewLine()
	{
		return TEXT("\r\n"); // 현재는 Windows만 지원하므로,
	}

	DWORD Environment::GetProcessorCount()
	{	
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		return sysinfo.dwNumberOfProcessors;	
	}

	_txstring Environment::BaseDirectory()
	{
		TCHAR modName[MAX_PATH];
		if(GetModuleFileNameEx(GetCurrentProcess(), NULL, modName, _countof(modName)) == 0)
		{
			// \todo 오류 처리 - GetLastError

		}

		_txstring directory = modName;
		return Path::GetDirectoryName(directory);
	}

}