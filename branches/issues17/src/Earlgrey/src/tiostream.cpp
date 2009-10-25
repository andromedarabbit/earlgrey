#include "stdafx.h"
#include "tiostream.h"

namespace Earlgrey
{

#ifdef _UNICODE
_tostream& _tcout = std::wcout;
#else
_tostream& _tcout = std::ccout;
#endif

}