// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#define _CRT_SECURE_DEPRECATE_MEMORY

#define NOMINMAX // min, max ��ũ�� ���ֱ�
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