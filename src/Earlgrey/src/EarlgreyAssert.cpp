#include "stdafx.h"
#include "EarlgreyAssert.h"

//! \note �ӽ� �ڵ�
#include <iostream>

namespace boost
{
	//! \note �ӽ� �ڵ�
	void assertion_failed(char const * expr, char const * function, char const * file, long line)
	{
		std::wcout << expr << function << file << line << std::endl;
	}
}
