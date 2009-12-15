#include "stdafx.h"
#include "Path.h"
#include "Environment.h"

using namespace Earlgrey;

namespace Earlgrey
{
	namespace Test
	{		
		TEST(PathTest, Combine)
		{
			_txstring path1 = TEXT("c:\\windows\\somewhere\\");
			_txstring path2 = TEXT("..\\");
			_txstring combined = Path::Combine(path1, path2);

			const TCHAR * const expected = TEXT("c:\\windows\\somewhere\\..\\");
			ASSERT_STREQ( 
				expected
				, combined.c_str()
				);
		}

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

		TEST(PathTest, Exists)
		{
			_txstring fullPath = Environment::BaseDirectory();
			fullPath = Path::Combine(fullPath, TEXT("..\\bin\\"));
									
			ASSERT_TRUE(Path::Exists(fullPath));
		}


		TEST(PathTest, NotExists)
		{
			_txstring fullPath = TEXT("c:\\somewhereovertherainbow");
			ASSERT_FALSE(Path::Exists(fullPath));
		}

		TEST(PathTest, IsDirectorySeparator)
		{
			ASSERT_TRUE(Path::IsDirectorySeparator(TEXT('/')));
			ASSERT_TRUE(Path::IsDirectorySeparator(TEXT('\\')));

			ASSERT_FALSE(Path::IsDirectorySeparator(TEXT('a')));
		}

		TEST(PathTest, IsPathRooted)
		{
			_txstring fileName = TEXT("C:\\mydir\\myfile.ext");
			_txstring uncPath = TEXT("\\\\mydir\\sudir\\");
			_txstring relativePath = TEXT("mydir\\sudir\\");
			_txstring relativePath2 = TEXT("..\\mydir\\subdir\\");

			ASSERT_TRUE(Path::IsPathRooted(fileName));
			ASSERT_TRUE(Path::IsPathRooted(uncPath));
			ASSERT_FALSE(Path::IsPathRooted(relativePath));
			ASSERT_FALSE(Path::IsPathRooted(relativePath2));
		}

		TEST(PathTest, GetFileNameWithTXString)
		{
			_txstring fullPath = TEXT("C:\\mydir\\myfile.ext");
			_txstring fileName = Path::GetFileName(fullPath);

			_txstring expected = TEXT("myfile.ext");
			ASSERT_STREQ(expected.c_str(), fileName.c_str());
		}

		TEST(PathTest, GetFileNameWithTString)
		{
			_tstring fullPath = TEXT("C:\\mydirectory\\mysubdirectory\\myfile");
			_tstring fileName = Path::GetFileName(fullPath);

			_tstring expected = TEXT("myfile");
			ASSERT_STREQ(expected.c_str(), fileName.c_str());
		}
	}
}

