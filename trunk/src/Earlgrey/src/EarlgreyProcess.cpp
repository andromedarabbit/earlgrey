#include "stdafx.h"
#include "EarlgreyProcess.h"
#include "RAII.h"

#include <tlhelp32.h>

namespace Earlgrey
{

	_txstring Process::MainModuleFileName()
	{
		TCHAR path[1024];

		if( GetModuleFileName( 0, path, 1023 ) == 0 ) {
			// TODO: need new exception class supporting a unicode msg 
			// const DWORD errCode = GetLastError();
			// _txstring msg = Log::ErrorMessage(errCode)
			throw std::exception("error occurs while retrieving name of main module.");
		}

		return _txstring(path);
	}


	//! \ref http://stackoverflow.com/questions/185254/how-can-a-win32-process-get-the-pid-of-its-parent
	//! \ref another implementation : http://www.codeproject.com/KB/threads/ParentPID.aspx
	DWORD Process::GetParentProcessID(DWORD pid)
	{
		handle_t h(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), CloseHandle);
		if(h.get() == INVALID_HANDLE_VALUE)
		{
			// TODO: GetLastError
			throw std::exception("Couldn't get the parent process ID");
		}

		PROCESSENTRY32 pe = { 0 };
		pe.dwSize = sizeof(PROCESSENTRY32);

		//assume first arg is the PID to get the PPID for, or use own PID
		if( Process32First(h.get(), &pe)) {
			do {
				if (pe.th32ProcessID == pid) {
					return pe.th32ParentProcessID;
				}
			} while( Process32Next(h.get(), &pe));
		}

		throw std::exception("No parent process found!");
	}
}