#include "stdafx.h"
#include "Log.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	_txstring Log::ErrorMessage(DWORD errorCode, HMODULE source)
	{
		__declspec(thread) static TCHAR msgBuf[1024 * 2];

		DWORD flags = // FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS
			;

		if(source != NULL)
			flags = FORMAT_MESSAGE_FROM_HMODULE | flags;

		DWORD retValue = FormatMessage (
			flags
			, source
			, errorCode
			, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT) // Default language
			, (LPTSTR) msgBuf
			, _countof(msgBuf) - 1
			, NULL
			);

		EARLGREY_VERIFY(retValue != 0);

		return _txstring(msgBuf);
	}

	_txstring Log::ErrorMessage(DWORD errorCode)
	{
		return ErrorMessage(errorCode, NULL);
	}
}