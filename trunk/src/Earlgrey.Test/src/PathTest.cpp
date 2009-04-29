#include "stdafx.h"
#include "Path.h"
#include "StlCustom.h"

using namespace Earlgrey;

namespace Earlgrey
{
	namespace Test
	{		
		TEST(PathTest, GetDirectoryName)
		{
			_txstring fullPath = TEXT("c:\\workspace\\earlgrey\\src\\Win32-Debug\\bin\\EarlgreyTest.exe");
			_txstring directory = Path::GetDirectoryName(fullPath);
			
			_txstring expected = TEXT("c:\\workspace\\earlgrey\\src\\Win32-Debug\\bin");
			ASSERT_STREQ(expected.c_str(), directory.c_str());
		}

		TEST(PathTest, GetDirectoryName2)
		{
			_txstring fullPath = TEXT("c:\\workspace\\earlgrey\\src\\Win32-Debug\\bin\\");
			_txstring directory = Path::GetDirectoryName(fullPath);

			_txstring expected = TEXT("c:\\workspace\\earlgrey\\src\\Win32-Debug\\bin");
			ASSERT_STREQ(expected.c_str(), directory.c_str());
		}

		/*
		TEST(PathTest, GetDirectoryName3)
		{
			_txstring fullPath = TEXT("c:\\workspace\\earlgrey\\src\\Win32-Debug\\bin");
			_txstring directory = Path::GetDirectoryName(fullPath);

			_txstring expected = TEXT("c:\\workspace\\earlgrey\\src\\Win32-Debug");
			ASSERT_STRNEQ(expected.c_str(), directory.c_str());
		}
		*/

		TEST(PathTest, Exists)
		{
			// _txstring fullPath = TEXT("..\\bin\\");
			_txstring fullPath = TEXT("c:\\workspace\\earlgrey\\src\\Win32-Debug\\bin\\");
						
			ASSERT_TRUE(Path::Exists(fullPath));
		}


		TEST(PathTest, NotExists)
		{
			// _txstring fullPath = TEXT("..\\bin\\");
			_txstring fullPath = TEXT("c:\\somewhereovertherainbow");

			ASSERT_FALSE(Path::Exists(fullPath));
		}


	}
}
