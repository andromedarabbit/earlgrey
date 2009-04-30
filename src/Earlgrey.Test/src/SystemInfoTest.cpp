#include "stdafx.h"
#include "SystemInfo.h"
#include "StlCustom.h"
#include "StringHelper.h"

using namespace Earlgrey;

namespace Earlgrey
{
	namespace Test
	{	
		//! \note 임시 조치
		TEST(SystemInfoTest, BaseDirectory)
		{
			SystemInfo info;
			
			// 예상값: "c:\workspace\earlgrey\src\Win32-Debug\bin"	
			_txstring directory = info.BaseDirectory();
			
			// 나온 값이 bin으로 끝나는지 확인한다.
			_txstring suffix = directory.substr(directory.length() - 3, 3);
			const TCHAR * const expected = TEXT("bin");
			ASSERT_TRUE(StringHelper::EndsWith(suffix, expected));
		}
		
	}
}
