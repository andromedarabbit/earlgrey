#include "stdafx.h"
#include "EarlgreyAssert.h"

//! \note 임시 코드
#include <iostream>

namespace boost
{
	//! \note 임시 코드
	//! \ref http://www.codeproject.com/KB/debug/DebugBreakAnyway.aspx
	void assertion_failed(char const * expr, char const * function, char const * file, long line)
	{
		std::wcout << expr << function << file << line << std::endl;

		::DebugBreak();
		// *((volatile INT *)0x1) = 1;
	}
}
