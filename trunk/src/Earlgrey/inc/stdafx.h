// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers


// TODO: reference additional headers your program requires here
#define _CRT_SECURE_DEPRECATE_MEMORY
#include <memory>

#include <stdlib.h>
#include <stdio.h>
#include <crtdbg.h>

#define NOMINMAX // min, max 매크로 없애기
#include <windows.h>

#include "EarlgreyPrivate.h"