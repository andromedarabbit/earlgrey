// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
// #define _CRT_SECURE_DEPRECATE_MEMORY

#include <Winsock2.h>

#define NOMINMAX
#include <windows.h>

#include <stdio.h>
#include <tchar.h>

#pragma warning( push )
#pragma warning( disable : 4512 )
#pragma warning( disable : 499 )
#pragma warning( disable : 4127 )
#include <gtest/gtest.h>
#pragma warning( pop )

#include "Uncopyable.h"
#include "txstring.h"
