#include "stdafx.h"
#include "tiostream.h"

namespace Earlgrey
{

#ifdef _UNICODE
_tistream& _tcin = std::wcin;
_tostream& _tcout = std::wcout;
_tostream& _tcerr = std::wcerr;
_tostream& _tclog = std::wclog;

#else
_tistream& _tcin = std::cin;
_tostream& _tcout = std::cout;
_tostream& _tcerr = std::cerr;
_tostream& _tclog = std::clog;
#endif

}