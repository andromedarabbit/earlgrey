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
		static _tstring MainModuleFileName();
		static void MainModuleFileName(TCHAR* path, DWORD length = MAX_PATH);

		static DWORD GetParentProcessID(DWORD pid);
		static _tstring GetParentProcessName(DWORD pid);
	};

}