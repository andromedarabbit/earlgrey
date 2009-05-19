#include "stdafx.h"
#include "Environment.h"
#include "Path.h"
#include <Psapi.h> // GetModuleFileNameEx

namespace Earlgrey 
{ 
	const TCHAR* const Environment::NewLine()
	{
		return TEXT("\r\n"); // ����� Windows�� �����ϹǷ�,
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
			// \todo ���� ó�� - GetLastError

		}

		_txstring directory = modName;
		return Path::GetDirectoryName(directory);
	}

}