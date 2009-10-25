#pragma once
#include <sstream>

namespace Earlgrey
{
#ifdef _UNICODE
	typedef std::wstringstream _tstringstream;
#else
	typedef	std::stringstream _tstringstream;
#endif
}