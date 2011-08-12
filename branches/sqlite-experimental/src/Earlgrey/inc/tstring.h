#pragma once

#include <string>

namespace Earlgrey
{

// TODO: std::wstring 을 earlgrey::wstring 으로 바꾸고, 그 다음에 다시  _tstring 으로 ....
#ifdef _UNICODE
typedef std::wstring _tstring;
#else
typedef	std::string _tstring;
#endif

}
