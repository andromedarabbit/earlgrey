#include "stdafx.h"
#include "EarlgreyProcess.h"
#include "RAII.h"

#include <tlhelp32.h>


namespace Earlgrey
{

	namespace 
	{
		//! \ref http://stackoverflow.com/questions/185254/how-can-a-win32-process-get-the-pid-of-its-parent
		//! \ref another implementation : http://www.codeproject.com/KB/threads/ParentPID.aspx
		BOOL GetParentProcessEntry(DWORD pid, PROCESSENTRY32& pe)
		{
			handle_t h(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0), CloseHandle);
			if(h.get() == INVALID_HANDLE_VALUE)
			{
				// TODO: GetLastError
				throw std::exception("Couldn't get the parent process ID");
			}

			//assume first arg is the PID to get the PPID for, or use own PID
			if( Process32First(h.get(), &pe)) {
				do {
					if (pe.th32ProcessID == pid) {
						return TRUE;
					}
				} while( Process32Next(h.get(), &pe) );
			}

			return FALSE;
		}

	}

	void Process::MainModuleFileName(TCHAR* path, DWORD length)
	{
		EARLGREY_ASSERT(path != NULL);
		EARLGREY_ASSERT(length > 0);

		if( GetModuleFileName( NULL, path, length ) == 0 ) {
			// TODO: need new exception class supporting a unicode msg 
			// const DWORD errCode = GetLastError();
			// _tstring msg = Log::ErrorMessage(errCode)
			throw std::exception("error occurs while retrieving name of main module.");
		}
	}

	_tstring Process::MainModuleFileName()
	{
		TCHAR path[MAX_PATH];
		MainModuleFileName(path, MAX_PATH);
		return _tstring(path);
	}


	DWORD Process::GetParentProcessID(DWORD pid)
	{
		PROCESSENTRY32 pe = { 0 };
		pe.dwSize = sizeof(PROCESSENTRY32);

		if(GetParentProcessEntry(pid, pe) == FALSE)
			throw std::exception("No parent process found!");
		return pe.th32ParentProcessID;
	}

	_tstring Process::GetParentProcessName(DWORD pid)
	{
		DWORD ppid = GetParentProcessID(pid);

		PROCESSENTRY32 pe = { 0 };
		pe.dwSize = sizeof(PROCESSENTRY32);

		if(GetParentProcessEntry(ppid, pe) == FALSE)
			throw std::exception("No parent process found!");
		return _tstring(pe.szExeFile);
	}
}