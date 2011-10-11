#pragma once
#include "Uncopyable.h"
#include "txstring.h"

namespace Earlgrey
{
	class Log 
		: private Uncopyable
	{
	private:
		explicit Log();

		static const DWORD BUFFER_LENGTH = 1024 * 64;
		__declspec(thread) static WCHAR MSG_BUFFER[BUFFER_LENGTH]; // The output buffer cannot be larger than 64K bytes.


	public: // class methods 
		//! \todo 임시 구현
		//! \ref http://recoverlee.tistory.com/9
		static const TCHAR * const ErrorMessage(DWORD errorCode, HMODULE source)
		{
#ifdef _UNICODE
			return ErrorMessageW(errorCode, source);
#else
			return ErrorMessageA(errorCode, source);
#endif
		}

		static const TCHAR * const ErrorMessage(DWORD errorCode)
		{
#ifdef _UNICODE
			return ErrorMessageW(errorCode);
#else
			return ErrorMessageA(errorCode);
#endif
		}


		static const WCHAR * const ErrorMessageW(DWORD errorCode, HMODULE source);
		static const WCHAR * const ErrorMessageW(DWORD errorCode);

		static const CHAR * const ErrorMessageA(DWORD errorCode, HMODULE source);
		static const CHAR * const ErrorMessageA(DWORD errorCode);
	};


}