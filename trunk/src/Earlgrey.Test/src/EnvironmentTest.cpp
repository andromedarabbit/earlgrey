#include "stdafx.h"
#include "Environment.h"
#include "StlCustom.h"
#include "StringHelper.h"
#include "Path.h"

#include <iostream>
using namespace Earlgrey;

namespace Earlgrey
{
	namespace Test
	{	
		inline void ASSERT_ENDS_WITH(const _txstring& expected, const _txstring& value)
		{
			const size_t valueLength = value.length();
			const size_t suffixLength = expected.length();			

			ASSERT_GE(valueLength, suffixLength);

			_txstring suffix = value.substr(value.length() - suffixLength, suffixLength);

			BOOL endsWith = StringHelper::EndsWith<STRCMP_CURRENT_CULTURE_IGNORECASE>(suffix, expected);
			ASSERT_TRUE(endsWith);
		}

		inline void ASSERT_CONTAINS(const _txstring& longStr, const _txstring& shortStr)
		{
			const size_t longLength = longStr.length();
			const size_t shortLength = shortStr.length();			

			ASSERT_GE(longLength, shortLength);

			size_t found = longStr.find(shortStr);
			ASSERT_TRUE(found != _txstring::npos);
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
			_txstring expected = TEXT("src");
			ASSERT_CONTAINS(directory, expected);
		}

		TEST(EnvironmentTest, SetCurrentDirectory)
		{
			// 예상값: "c:\workspace\earlgrey\trunk\src\Earlgrey.Test"
			_txstring oldDirectory = Environment::CurrentDirectory();
			_txstring newDirectory = Path::Combine(oldDirectory, TEXT(".."));

			ASSERT_TRUE(Environment::CurrentDirectory(newDirectory));
			_txstring changedDirectory = Environment::CurrentDirectory();
			_txstring expected = TEXT("src");
			ASSERT_CONTAINS(changedDirectory, expected);

			ASSERT_TRUE(Environment::CurrentDirectory(oldDirectory));
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
	}
}
