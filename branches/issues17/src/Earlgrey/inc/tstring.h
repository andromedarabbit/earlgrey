#pragma once

#include <string>

namespace Earlgrey
{

#ifdef _UNICODE
typedef std::wstring _tstring;
#else
typedef	std::string _tstring;
#endif

}
