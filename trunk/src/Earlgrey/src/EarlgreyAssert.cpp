#include "stdafx.h"
#include "EarlgreyAssert.h"

#include "tiostream.h"

namespace Earlgrey
{
	using namespace std;

	//! \note 임시 코드
	//! \ref http://www.codeproject.com/KB/debug/DebugBreakAnyway.aspx
	void assertion_failed(TCHAR const * expr, TCHAR const * function, TCHAR const * file, long line)
	{
		// \note 유니코드란 가정이라 버그라 봐야 하지만 어차피 임시 코드이니 놔둔다.
		_tcout
			<< expr << std::endl 
			<< function << std::endl
			<< file << std::endl
			<< line << std::endl; 

		::DebugBreak();
		// *((volatile INT *)0x1) = 1;
	}
}
