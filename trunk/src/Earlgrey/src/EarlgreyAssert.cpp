#include "stdafx.h"
#include "EarlgreyAssert.h"

#include "tiostream.h"

namespace Earlgrey
{
	using namespace std;

	//! \note �ӽ� �ڵ�
	//! \ref http://www.codeproject.com/KB/debug/DebugBreakAnyway.aspx
	void assertion_failed(TCHAR const * expr, TCHAR const * function, TCHAR const * file, long line)
	{
		// \note �����ڵ�� �����̶� ���׶� ���� ������ ������ �ӽ� �ڵ��̴� ���д�.
		_tcout
			<< expr << std::endl 
			<< function << std::endl
			<< file << std::endl
			<< line << std::endl; 

		::DebugBreak();
		// *((volatile INT *)0x1) = 1;
	}
}
