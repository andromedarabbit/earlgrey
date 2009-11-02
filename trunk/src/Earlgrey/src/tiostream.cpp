#include "stdafx.h"
#include "tiostream.h"

namespace Earlgrey
{

#ifdef _UNICODE
_tostream& _tcout = std::wcout;
_tostream& _tcerr = std::wcerr;
_tistream& _tcin = std::wcin;
#else
_tostream& _tcout = std::cout;
_tostream& _tcerr = std::cerr;
_tistream& _tcin = std::cin;
#endif

}