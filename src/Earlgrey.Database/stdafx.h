// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#define _CRT_SECURE_DEPRECATE_MEMORY

#define NOMINMAX // min, max 매크로 없애기
#include <windows.h>
#include <tchar.h>

#if (_MSC_VER >= 1600)
#	pragma warning(push)
#	pragma warning(disable: 4996)
#	include <memory>
#	pragma warning(pop)
#else
#	include <memory>
#endif