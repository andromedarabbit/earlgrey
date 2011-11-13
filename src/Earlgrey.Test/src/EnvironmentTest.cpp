#include "stdafx.h"
#include "Environment.h"
#include "StringHelper.h"
#include "Path.h"

#include <iostream>

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
			_txstring expected = TEXT("bin");
			ASSERT_ENDS_WITH(expected, directory);
		}

		TEST(EnvironmentTest, GetCurrentDirectoryApi)
		{
			DWORD bufferSize = 1;

			// TCHAR dirName[bufferSize] { TEXT('0'), }; 
			TCHAR * dirName = NULL;
			for(;;)
			{
				dirName = new TCHAR[bufferSize];
				dirName[0] = TEXT('0');

				DWORD returnedSize = GetCurrentDirectory(bufferSize, dirName);
				if(returnedSize == 0)
				{
					const DWORD errCode = GetLastError();
					const char * errMsg = Log::ErrorMessageA(errCode);
					throw std::exception(errMsg);
				}

				if(returnedSize == _tcslen(dirName))
					break;

				delete dirName;
				bufferSize = returnedSize;
				// dirName = new TCHAR[bufferSize];
			}

			delete dirName;

		}

		TEST(EnvironmentTest, GetCurrentDirectory)
		{
			// 예상값: "c:\workspace\earlgrey\trunk\src\Earlgrey.Test"
			_txstring directory = Environment::CurrentDirectory();
			_txstring expected = TEXT("src");
			ASSERT_CONTAINS(directory, expected);
		}

		TEST(EnvironmentTest, GetSystemDirectory)
		{
			// 예상값: "c:\workspace\earlgrey\trunk\src\Earlgrey.Test"
			_txstring directory = Environment::SystemDirectory();
			_txstring expected = TEXT("Windows\\system32");
			ASSERT_CONTAINS(directory, expected);
		}

		
		TEST(EnvironmentTest, SetCurrentDirectory)
		{
			// 예상값: "c:\workspace\earlgrey\trunk\src\Earlgrey.Test"
			_txstring oldDirectory = Environment::CurrentDirectory();
			_txstring newDirectory = Path::Combine(oldDirectory, TEXT(".."));

			ASSERT_TRUE2(Environment::CurrentDirectory(newDirectory));
			_txstring changedDirectory = Environment::CurrentDirectory();
			_txstring expected = TEXT("src");
			ASSERT_CONTAINS(changedDirectory, expected);

			ASSERT_TRUE2(Environment::CurrentDirectory(oldDirectory));
			changedDirectory = Environment::CurrentDirectory();
			expected = TEXT("src");
			ASSERT_CONTAINS(changedDirectory, expected);
		}

		TEST(EnvironmentTest, SetCurrentDirectoryWithEmptyString)
		{
			ASSERT_THROW(Environment::CurrentDirectory(_TEXT("")), std::invalid_argument);
		}

		TEST(EnvironmentTest, MachineName)
		{			
			_txstring name = Environment::MachineName();
			ASSERT_TRUE(name.length() > 0);
		}

		TEST(EnvironmentTest, UserDomainName)
		{		
			_txstring name = Environment::UserDomainName();
			ASSERT_TRUE(name.length() > 0);
		}

		TEST(EnvironmentTest, UserInteractive)
		{
			ASSERT_TRUE2( Environment::UserInteractive() );
		}

		TEST(EnvironmentTest, TickCount)
		{
			ASSERT_GT( Environment::TickCount(), static_cast<DWORD>(0)  );
		}

#if _WIN32_WINNT >= 0x0600
		TEST(EnvironmentTest, TickCount64)
		{
			ASSERT_GT( Environment::TickCount64(), static_cast<ULONGLONG>(0) );
		}
#endif
		
		TEST(EnvironmentTest, GetPerformaceCounter)
		{
			EXPECT_GT( Environment::GetPerformanceCounter(), static_cast<__int64>(0) );
			EXPECT_GT( Environment::GetPerformanceFrequency(), static_cast<__int64>(0) );
		}
	}
}
