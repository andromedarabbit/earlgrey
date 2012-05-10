#include "stdafx.h"
#include "Log.h"
#include "EarlgreyAssert.h"
#include "StringHelper.h"

namespace Earlgrey
{
	namespace 
	{
		void GetErrorMessageW(DWORD errorCode, HMODULE source, WCHAR * msgBuffer, DWORD bufferLength)
		{
			DWORD flags = // FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM |
				FORMAT_MESSAGE_IGNORE_INSERTS
				;

			if(source != NULL)
				flags = FORMAT_MESSAGE_FROM_HMODULE | flags;

			DWORD retValue = FormatMessageW (
				flags
				, source
				, errorCode
				, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT) // Default language
				, msgBuffer
				, bufferLength - 1
				, NULL
				);

			EARLGREY_VERIFY(retValue != 0);
		}
	}

	WCHAR Log::MSG_BUFFER[BUFFER_LENGTH];


	const TCHAR * const Log::ErrorMessage(DWORD errorCode, HMODULE source)
	{
#ifdef _UNICODE
		return ErrorMessageW(errorCode, source);
#else
		return ErrorMessageA(errorCode, source);
#endif
	}

	const TCHAR * const Log::ErrorMessage(DWORD errorCode)
	{
#ifdef _UNICODE
		return ErrorMessageW(errorCode);
#else
		return ErrorMessageA(errorCode);
#endif
	}

	/*xwstring Log::ErrorMessageW(DWORD errorCode, HMODULE source)
	{		
		GetErrorMessageW(errorCode, source, MSG_BUFFER, BUFFER_LENGTH);
		return xwstring(MSG_BUFFER);
	}

	xwstring Log::ErrorMessageW(DWORD errorCode)
	{
		return ErrorMessageW(errorCode, NULL);
	}*/

	const WCHAR * const Log::ErrorMessageW(DWORD errorCode, HMODULE source)	
	{		
		GetErrorMessageW(errorCode, source, MSG_BUFFER, BUFFER_LENGTH);
		return MSG_BUFFER;
	}

	const WCHAR * const Log::ErrorMessageW(DWORD errorCode)
	{
		return ErrorMessageW(errorCode, NULL);
	}


	const CHAR * const Log::ErrorMessageA(DWORD errorCode, HMODULE source)
	{
		GetErrorMessageW(errorCode, source, MSG_BUFFER, BUFFER_LENGTH);
		const int msgSize = EARLGREY_NUMERIC_CAST<int>(wcslen(MSG_BUFFER) * sizeof(WCHAR));
		return String::FromUnicode(MSG_BUFFER, msgSize);
	}

	const CHAR * const Log::ErrorMessageA(DWORD errorCode)
	{
		return ErrorMessageA(errorCode, NULL);
	}
}