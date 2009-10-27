#include "stdafx.h"
#include "EarlgreyProcess.h"

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


}