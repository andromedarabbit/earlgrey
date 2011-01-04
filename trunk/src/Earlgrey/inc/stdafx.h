// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define _CRT_SECURE_DEPRECATE_MEMORY

#if (_MSC_VER >= 1600)
#	pragma warning(push)
#	pragma warning(disable: 4996)
#	include <memory>
#	pragma warning(pop)
#else
#	include <memory>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>

#define NOMINMAX // min, max 매크로 없애기
#include <WinSock2.h>
#include <windows.h>

#include "EarlgreyPrivate.h"