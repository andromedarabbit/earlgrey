#pragma  once

#if defined(_WIN32) || defined(_WIN64)
#include "PAL_Windows.h"
#else   
#error "아직은 다른 OS 는 지원하지 않습니다."
#endif

#include <tchar.h>


// 미리 컴파일된 헤더엔 정말 많이 쓰는 헤더만 넣었으면 합니다.
#include "EarlgreyAssert.h"
#include "txstring.h"
