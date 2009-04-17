#pragma  once

#if defined(_WIN32) || defined(_WIN64)
#include "PAL_Windows.h"
#else
#error "아직은 다른 OS 는 지원하지 않습니다."
#endif

#include "LockfreePointer.h"
#include "Lockfree.h"

namespace Earlgrey
{

}

