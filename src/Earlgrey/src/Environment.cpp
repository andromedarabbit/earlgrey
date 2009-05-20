#include "stdafx.h"
#include "Environment.h"
#include "Path.h"
#include <Psapi.h> // GetModuleFileNameEx
#include <Lmcons.h> // UNLEN

namespace Earlgrey 
{ 
	const TCHAR* const Environment::NewLine()
	{
		return TEXT("\r\n"); // 현재는 Windows만 지원하므로,
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
			// \todo 오류 처리 - GetLastError

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
			// \todo 오류 처리 - GetLastError

		}
	
		_txstring directory(dirName);
		return directory;
	}

	//! 현재는 throw 처리를 하는데 그럴거면 BOOL가 아닌 void 반환이 나을 듯. GetLastError로 오류를 확인하면 되니까.
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
			// \todo 오류 처리 - GetLastError

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
			// \todo 오류 처리 - GetLastError

		}
	
		return name;
	}


}