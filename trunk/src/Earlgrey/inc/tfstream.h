#pragma once
#include "tios.h"
#include <iostream>

namespace Earlgrey
{
#ifdef _UNICODE
	typedef std::wfstream _tfstream;
	typedef std::wofstream _tofstream;
	typedef std::wistream _tistream;
#else
	typedef std::fstream _tfstream;
	typedef	std::ofstream _tofstream;
	typedef std::istream _tistream;
#endif

}

