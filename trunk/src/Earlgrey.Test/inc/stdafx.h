// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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
