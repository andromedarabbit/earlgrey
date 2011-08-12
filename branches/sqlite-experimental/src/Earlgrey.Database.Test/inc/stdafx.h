// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#define _CRT_SECURE_DEPRECATE_MEMORY

#include <Winsock2.h>

#define NOMINMAX
#include <windows.h>

#include <stdio.h>
#include <tchar.h>

#pragma warning( push )
#pragma warning( disable : 4512 )
#pragma warning( disable : 499 )
#pragma warning( disable : 4127 )
#if (_MSC_VER >= 1600)
#	pragma warning(disable: 4996)
#	include <gtest/gtest.h>
#else
#	include <gtest/gtest.h>
#endif
#pragma warning( pop )

#include "Uncopyable.h"
#include "txstring.h"
