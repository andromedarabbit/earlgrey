#pragma  once

#if defined(_WIN32) || defined(_WIN64)
#include "PAL_Windows.h"
#else   
#error "������ �ٸ� OS �� �������� �ʽ��ϴ�."
#endif

#include <tchar.h>


// �̸� �����ϵ� ����� ���� ���� ���� ����� �־����� �մϴ�.
#include "EarlgreyAssert.h"
#include "txstring.h"
