#include "stdafx.h"
#include "SystemInfo.h"
#include "StlCustom.h"

using namespace Earlgrey;

namespace Earlgrey
{
	namespace Test
	{	
		//! \note �ӽ� ��ġ
		TEST(SystemInfoTest, BaseDirectory)
		{
			SystemInfo info;
			
			// ����: "c:\workspace\earlgrey\src\Win32-Debug\bin"	
			_txstring directory = info.BaseDirectory();
			
			// ���� ���� bin���� �������� Ȯ���Ѵ�.
			// std::size_t found = directory.find(TEXT("bin"));
			// ASSERT_NE(_txstring::npos, found);
			// ASSERT_EQ(directory.size(), found + 3);

			_txstring suffix = directory.substr(directory.length() - 3, 3);
			const TCHAR * const expected = TEXT("bin");
			// ASSERT_STREQ(expected, suffix.c_str());

			ASSERT_EQ(CSTR_EQUAL, CompareString(GetThreadLocale(), NORM_IGNORECASE, expected, -1, suffix.c_str(), -1));
		}
		
	}
}
