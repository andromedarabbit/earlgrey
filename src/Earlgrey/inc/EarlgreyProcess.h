#pragma once
#include "Uncopyable.h"
#include "tstring.h"

namespace Earlgrey
{
	class Process : private Uncopyable
	{
	private:
		explicit Process();

	public:

#ifdef _UNICODE
		#define MainModuleFileName       MainModuleFileNameW		
#else
		#define MainModuleFileName       MainModuleFileNameA
#endif
		static std::string MainModuleFileNameA();
		static std::wstring MainModuleFileNameW();

		static void MainModuleFileNameA(CHAR* path, DWORD length = MAX_PATH);
		static void MainModuleFileNameW(WCHAR* path, DWORD length = MAX_PATH);

		static DWORD GetParentProcessID(DWORD pid);
		static _tstring GetParentProcessName(DWORD pid);
	};

}