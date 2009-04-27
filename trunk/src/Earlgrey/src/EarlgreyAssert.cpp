#include "stdafx.h"
#include "EarlgreyAssert.h"

//! \note �ӽ� �ڵ�
#include <iostream>

namespace boost
{
	//! \note �ӽ� �ڵ�
	//! \ref http://www.codeproject.com/KB/debug/DebugBreakAnyway.aspx
	void assertion_failed(char const * expr, char const * function, char const * file, long line)
	{
		std::wcout << expr << function << file << line << std::endl;

		::DebugBreak();
		// *((volatile INT *)0x1) = 1;
	}
}
