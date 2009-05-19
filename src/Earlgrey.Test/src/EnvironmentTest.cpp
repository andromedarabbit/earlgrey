#include "stdafx.h"
#include "Environment.h"
#include "StlCustom.h"
#include "StringHelper.h"

using namespace Earlgrey;

namespace Earlgrey
{
	namespace Test
	{	
		//! \note 임시 조치
		TEST(EnvironmentTest, BaseDirectory)
		{	
			// 예상값: "c:\workspace\earlgrey\src\Win32-Debug\bin"	
			_txstring directory = Environment::BaseDirectory();
			
			// 나온 값이 bin으로 끝나는지 확인한다.
			_txstring suffix = directory.substr(directory.length() - 3, 3);
			const TCHAR * const expected = TEXT("bin");
			ASSERT_TRUE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE_IGNORECASE>(suffix, expected));
		}
		
	}
}
