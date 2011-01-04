// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"


// TODO: reference additional headers your program requires here
#define _CRT_SECURE_DEPRECATE_MEMORY


#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <crtdbg.h>

#pragma warning(push)
#pragma warning(disable: 4996)
#include <memory>
#pragma warning(pop)

#include <Winsock2.h>

#define NOMINMAX // min, max 매크로 없애기
#include <windows.h>

#include <map>
#include <vector>
#include <set>

#include "Earlgrey.h"
#include "loki/Singleton.h"
#include "xmap.h"
#include "ScopedLock.h"