#include "stdafx.h"
#include "Environment.h"
#include "OperatingSystem.h"
#include "Path.h"

#include <Psapi.h> // GetModuleFileNameEx
#include <Lmcons.h> // UNLEN

#pragma comment(lib, "Psapi.lib")

namespace Earlgrey 
{ 

	//BOOL Environment::s_Initialized = FALSE;
	//Environment::mutex_type Environment::s_InternalSyncObject;

	const TCHAR* const Environment::NewLine()
	{
		return TEXT("\r\n"); // 현재는 Windows만 지원하므로,
	}

	
	//! \ref http://www.tipssoft.com/bulletin/board.php?bo_table=QnA&wr_id=2395
	_txstring Environment::BaseDirectory()
	{
		// TODO: 버퍼 가변 사이즈로
		const DWORD bufferSize = MAX_PATH + 1;

		TCHAR modName[bufferSize];
		if(GetModuleFileNameEx(GetCurrentProcess(), NULL, modName, _countof(modName)) == 0)
		{
			const DWORD errCode = GetLastError();
			const char * errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}

		return Path::GetDirectoryName(modName);
	}

	/*

	DWORD bufferSize = 1;

	// TCHAR dirName[bufferSize] { TEXT('0'), }; 
	TCHAR * dirName = NULL;
	for(;;)
	{
	dirName = new TCHAR[bufferSize];
	dirName[0] = TEXT('0');

	DWORD returnedSize = GetCurrentDirectory(bufferSize, dirName);
	if(returnedSize == 0)
	{
	const DWORD errCode = GetLastError();
	const char * errMsg = Log::ErrorMessageA(errCode);
	throw std::exception(errMsg);
	}

	if(returnedSize == _tcslen(dirName))
	break;

	delete dirName;
	bufferSize = returnedSize;
	// dirName = new TCHAR[bufferSize];
	}

	delete dirName;
	*/

	_txstring Environment::CurrentDirectory()
	{
		//// TODO: 버퍼 가변 사이즈로
		//const DWORD bufferSize = MAX_PATH + 1;

		//TCHAR dirName[bufferSize]; 
		//if(GetCurrentDirectory(bufferSize, dirName) == 0)
		//{
		//	const DWORD errCode = GetLastError();
		//	const char * errMsg = Log::ErrorMessageA(errCode);
		//	throw std::exception(errMsg);
		//}
	
		//return _txstring(dirName);

		DWORD bufferSize = MAX_PATH;

		// TCHAR dirName[bufferSize] { TEXT('0'), }; 
		TCHAR * dirName = NULL;
		for(;;)
		{
			dirName = Malloca<TCHAR>(bufferSize);
			dirName[0] = TEXT('0');

			DWORD returnedSize = GetCurrentDirectory(bufferSize, dirName);
			if(returnedSize == 0)
			{
				const DWORD errCode = GetLastError();
				const char * errMsg = Log::ErrorMessageA(errCode);
				throw std::exception(errMsg);
			}

			if(returnedSize == _tcslen(dirName))
				break;

			// delete dirName;
			Freea(dirName);
			bufferSize = returnedSize;
			// dirName = new TCHAR[bufferSize];
		}

		// delete dirName;
		// Freea(dirName);
		return dirName;
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


	_txstring Environment::SystemDirectory()
	{
		// TODO: 버퍼 가변 사이즈로
		const DWORD bufferSize = MAX_PATH + 1;

		TCHAR dirName[bufferSize]; 
		if(GetSystemDirectory(dirName, bufferSize) == 0)
		{
			const DWORD errCode = GetLastError();
			const char * errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}

		return _txstring(dirName);
	}



	_txstring Environment::MachineName()
	{
		const DWORD bufferSize = MAX_COMPUTERNAME_LENGTH + 1;

		TCHAR name[bufferSize]; 
		
		DWORD size = bufferSize;
		if(GetComputerName(name, &size) == 0)
		{
			const DWORD errCode = GetLastError();
			const char * errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
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
			const DWORD errCode = GetLastError();
			const char * errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}
	
		return name;
	}

	
	//! \ref http://msdn2.microsoft.com/en-us/library/ms724429.aspx
	//! \ref http://msdn.microsoft.com/en-us/library/ms724834%28VS.85%29.aspx

	BOOL Environment::UserInteractive() throw(...)
	{		
		/*if ((OSInfo() & OSNAME_WINNT) != OSNAME_WINNT)
			throw std::exception("");*/
		if(OperatingSystem::IsWinNt() == FALSE)
			throw std::exception("method not supported by this version of Windows");
		
		HWINSTA hStation = GetProcessWindowStation ( ) ;
		if (hStation == NULL)
		{
			const DWORD errCode = GetLastError();
			const char * errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}
		
		USEROBJECTFLAGS stUOF;
		DWORD dwNeeded ;
		if(GetUserObjectInformation (hStation,	UOI_FLAGS, &stUOF, sizeof ( USEROBJECTFLAGS), &dwNeeded) == 0)
		{
			const DWORD errCode = GetLastError();
			const char * errMsg = Log::ErrorMessageA(errCode);
			throw std::exception(errMsg);
		}

		return (( WSF_VISIBLE & stUOF.dwFlags ) == WSF_VISIBLE);
	}

	DWORD Environment::TickCount()
	{
		return ::GetTickCount();
	}

#if (_WIN32_WINNT >= 0x0600)
	ULONGLONG Environment::TickCount64()
	{
		return ::GetTickCount64();
	}
#else
	ULONGLONG Environment::TickCount64()
	{
		throw std::exception("not supported in this Windows!");
	}
#endif

	__int64 Environment::GetPerformanceCounter()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		return li.QuadPart;
	}

	__int64 Environment::GetPerformanceFrequency()
	{
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		return li.QuadPart;
	}
}
