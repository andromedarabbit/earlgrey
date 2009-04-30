#include "stdafx.h"
#include "EarlgreyAssert.h"

#include <iostream> //! \note �ӽ� �ڵ�

namespace Earlgrey
{
	//! \note �ӽ� �ڵ�
	//! \ref http://www.codeproject.com/KB/debug/DebugBreakAnyway.aspx
	void assertion_failed(TCHAR const * expr, TCHAR const * function, TCHAR const * file, long line)
	{
		// \note �����ڵ�� �����̶� ���׶� ���� ������ ������ �ӽ� �ڵ��̴� ���д�.
		std::wcout 
			<< expr << std::endl 
			<< function << std::endl
			<< file << std::endl
			<< line << std::endl; 

		::DebugBreak();
		// *((volatile INT *)0x1) = 1;
	}
}
