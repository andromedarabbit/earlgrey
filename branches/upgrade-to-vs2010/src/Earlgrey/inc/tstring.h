#pragma once

#include <string>

namespace Earlgrey
{

// TODO: std::wstring �� earlgrey::wstring ���� �ٲٰ�, �� ������ �ٽ�  _tstring ���� ....
#ifdef _UNICODE
typedef std::wstring _tstring;
#else
typedef	std::string _tstring;
#endif

}
