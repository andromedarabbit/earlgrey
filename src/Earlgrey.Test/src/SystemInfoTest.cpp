#include "stdafx.h"
#include "SystemInfo.h"
#include "StlCustom.h"
#include "StringHelper.h"

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
			_txstring suffix = directory.substr(directory.length() - 3, 3);
			const TCHAR * const expected = TEXT("bin");
			ASSERT_TRUE(StringHelper::EndsWith(suffix, expected));
		}
		
	}
}
