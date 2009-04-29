#include "stdafx.h"
#include "SystemInfo.h"
#include "StlCustom.h"

using namespace Earlgrey;

namespace Earlgrey
{
	namespace Test
	{	
		//! \note 임시 조치
		/*
		TEST(SystemInfoTest, BaseDirectory)
		{
			SystemInfo info;
			
			// 예상값: "c:\workspace\earlgrey\src\Win32-Debug\bin"	
			_txstring directory = info.BaseDirectory();
			std::size_t found = directory.find(TEXT("bin"));
	
			// 나온 값이 bin으로 끝나는지 확인한다.
			ASSERT_NE(_txstring::npos, found);
			ASSERT_EQ(directory.size(), found + 3);
		}
		*/
	}
}
