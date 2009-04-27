#include "stdafx.h"
#include "EarlgreyAssert.h"

//! \note 임시 코드
#include <iostream>

namespace boost
{
	//! \note 임시 코드
	void assertion_failed(char const * expr, char const * function, char const * file, long line)
	{
		std::wcout << expr << function << file << line << std::endl;
	}
}
