#include "stdafx.h"
#include "Environment.h"
#include "StlCustom.h"
#include "StringHelper.h"
#include "Path.h"

using namespace Earlgrey;

namespace Earlgrey
{
	namespace Test
	{	
		void ASSERT_ENDS_WITH(const _txstring& expected, const _txstring& value)
		{
			size_t suffixLength = expected.length();

			_txstring suffix = value.substr(value.length() - suffixLength, suffixLength);

			ASSERT_TRUE(StringHelper::EndsWith<STRCMP_CURRENT_CULTURE_IGNORECASE>(suffix, expected));
		}

		//! \note 임시 조치
		TEST(EnvironmentTest, BaseDirectory)
		{	
			// 예상값: "c:\workspace\earlgrey\src\Win32-Debug\bin"	
			_txstring directory = Environment::BaseDirectory();
			
			// 나온 값이 bin으로 끝나는지 확인한다.
			_txstring expected = TEXT("bin");
			ASSERT_ENDS_WITH(expected, directory);
		}

		TEST(EnvironmentTest, ProcessorCount)
		{
			DWORD count = Environment::ProcessorCount();
			DWORD expected = 1;
			ASSERT_GE(count, expected);
		}
		
		TEST(EnvironmentTest, GetCurrentDirectory)
		{
			// 예상값: "c:\workspace\earlgrey\trunk\src\Earlgrey.Test"
			_txstring directory = Environment::CurrentDirectory();
			_txstring expected = TEXT("Earlgrey.Test");
			ASSERT_ENDS_WITH(expected, directory);
		}

		TEST(EnvironmentTest, SetCurrentDirectory)
		{
			// 예상값: "c:\workspace\earlgrey\trunk\src\Earlgrey.Test"
			_txstring oldDirectory = Environment::CurrentDirectory();
			_txstring newDirectory = Path::Combine(oldDirectory, TEXT(".."));

			ASSERT_TRUE(Environment::CurrentDirectory(newDirectory));
			_txstring changedDirectory = Environment::CurrentDirectory();
			_txstring expected = TEXT("src");
			ASSERT_ENDS_WITH(expected, changedDirectory);

			ASSERT_TRUE(Environment::CurrentDirectory(oldDirectory));
			changedDirectory = Environment::CurrentDirectory();
			expected = TEXT("Earlgrey.Test");
			ASSERT_ENDS_WITH(expected, changedDirectory);
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
	}
}
