#pragma  once

#if defined(_WIN32) || defined(_WIN64)
#include "PAL_Windows.h"
#else
#error "아직은 다른 OS 는 지원하지 않습니다."
#endif

#include <tchar.h>


// 당분간 미리 컴파일된 헤더를 최대한 안 썼으면 좋겠습니다. 