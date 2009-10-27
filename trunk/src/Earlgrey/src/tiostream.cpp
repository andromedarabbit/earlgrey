#include "stdafx.h"
#include "tiostream.h"

namespace Earlgrey
{

#ifdef _UNICODE
_tostream& _tcout = std::wcout;
#else
_tostream& _tcout = std::cout;
#endif

#ifdef _UNICODE
_tostream& _tcerr = std::wcerr;
#else
_tostream& _tcerr = std::cerr;
#endif

}