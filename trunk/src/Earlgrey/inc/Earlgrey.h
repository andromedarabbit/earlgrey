#pragma  once

#if defined(_WIN32) || defined(_WIN64)
#include "PAL_Windows.h"
#else
#error "������ �ٸ� OS �� �������� �ʽ��ϴ�."
#endif

#include "LockfreePointer.h"
#include "Lockfree.h"

namespace Earlgrey
{

}

