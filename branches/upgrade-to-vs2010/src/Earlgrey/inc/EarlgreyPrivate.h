#pragma  once

#include <WinSock2.h>

#if defined(_WIN32) || defined(_WIN64)
#include "PAL_Windows.h"
#else   
#error "������ �ٸ� OS �� �������� �ʽ��ϴ�."
#endif

#include <tchar.h>
#include <map>
#include <vector>
#include <new>
#include <exception>

#if (_MSC_VER >= 1600)
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

#include <string>

#if (_MSC_VER >= 1600)
#pragma warning(pop)
#endif

#include <queue>
#include <array>

// �̸� �����ϵ� ����� ���� ���� ���� ����� �־����� �մϴ�.
#include "shared_ptr_helper.h"
#include "Log.h"
#include "Logger.h"
#include "BuildConfiguration.h"
#include "EarlgreyAssert.h"
#include "txstring.h"
#include "Proactor.h"
#include "Socket.h"
#include "Runnable.h"
