#include "stdafx.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	using namespace std;

	//! \note 임시 코드
	//! \ref http://www.codeproject.com/KB/debug/DebugBreakAnyway.aspx
	void assertion_failed(TCHAR const * expr, TCHAR const * function, TCHAR const * file, long line)
	{
		DBG_UNREFERENCED_PARAMETER(expr);
		DBG_UNREFERENCED_PARAMETER(function);
		DBG_UNREFERENCED_PARAMETER(file);
		DBG_UNREFERENCED_PARAMETER(line);

		/*if( GlobalExceptionHandler::Initialized() == TRUE )
		{
			*(volatile INT *)0x1) = 1;
			return;
		}
		*/	
		*((volatile INT *)0x1) = 1;
	}
}
