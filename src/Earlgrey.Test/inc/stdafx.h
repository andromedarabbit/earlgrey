// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#define NOMINMAX
#include <Winsock2.h>
#include <windows.h>


#pragma warning( push )
#pragma warning( disable : 4512 )
#pragma warning( disable : 499 )
#pragma warning( disable : 4127 )

#include <gtest/gtest.h>

#pragma warning( pop )


#include "Uncopyable.h"
#include "txstring.h"
