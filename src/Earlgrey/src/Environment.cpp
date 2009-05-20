#include "stdafx.h"
#include "Environment.h"
#include "Path.h"
#include <Psapi.h> // GetModuleFileNameEx
#include <Lmcons.h> // UNLEN

namespace Earlgrey 
{ 
	const TCHAR* const Environment::NewLine()
	{
		return TEXT("\r\n"); // ����� Windows�� �����ϹǷ�,
	}

	DWORD Environment::ProcessorCount()
	{	
		SYSTEM_INFO sysinfo;
		GetSystemInfo(&sysinfo);
		return sysinfo.dwNumberOfProcessors;	
	}

	//! \ref http://www.tipssoft.com/bulletin/board.php?bo_table=QnA&wr_id=2395
	_txstring Environment::BaseDirectory()
	{
		const DWORD bufferSize = MAX_PATH + 1;

		TCHAR modName[bufferSize];
		if(GetModuleFileNameEx(GetCurrentProcess(), NULL, modName, _countof(modName)) == 0)
		{
			// \todo ���� ó�� - GetLastError

		}

		_txstring directory(modName);
		return Path::GetDirectoryName(directory);
	}

	_txstring Environment::CurrentDirectory()
	{
		const DWORD bufferSize = MAX_PATH + 1;

		TCHAR dirName[bufferSize]; 
		if(GetCurrentDirectory(bufferSize, dirName) == 0)
		{
			// \todo ���� ó�� - GetLastError

		}
	
		_txstring directory(dirName);
		return directory;
	}

	//! ����� throw ó���� �ϴµ� �׷��Ÿ� BOOL�� �ƴ� void ��ȯ�� ���� ��. GetLastError�� ������ Ȯ���ϸ� �Ǵϱ�.
	BOOL Environment::CurrentDirectory(const _txstring& pathName) throw(...)
	{
		if(pathName.empty())
		{
			throw std::invalid_argument("Path shouldn't be an empty string!");
		}

		if(pathName.length() > MAX_PATH)
		{
			throw std::invalid_argument("The length of the directory path is too long!");
		}

		return SetCurrentDirectory(pathName.c_str());
	}

	_txstring Environment::MachineName()
	{
		const DWORD bufferSize = MAX_COMPUTERNAME_LENGTH + 1;

		TCHAR name[bufferSize]; 
		
		DWORD size = bufferSize;
		if(GetComputerName(name, &size) == 0)
		{
			// \todo ���� ó�� - GetLastError

		}

		return name;
	}

	_txstring Environment::UserDomainName()
	{
		const DWORD bufferSize = UNLEN  + 1;

		TCHAR name[bufferSize]; 

		DWORD size = bufferSize;
		if(GetUserName(name, &size) == 0)
		{
			// \todo ���� ó�� - GetLastError

		}
	
		return name;
	}


}