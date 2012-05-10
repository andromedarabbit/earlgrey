#include "stdafx.h"
#include "FileVersionInfo.h"
#include "EarlgreyProcess.h"
#include "txstring.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(FileVersionInfoTest, GetVersionInfo)
		{
			std::wstring fileName( Process::MainModuleFileNameW() );

			FileVersionInfo versionInfo( FileVersionInfo::GetVersionInfo(fileName) );
			std::wstring fileVersion = versionInfo.FileVersion();
			ASSERT_STARTS_WITH( L"1.0.0.", fileVersion.c_str() );

			std::wstring productName = versionInfo.ProductName();
			ASSERT_STREQ( L"ProductName: Earlgrey", productName.c_str() );

			std::wstring fileDescription = versionInfo.FileDescription();
			ASSERT_STREQ( L"FileDescription: Earlgrey", fileDescription.c_str() );
			
		}

		TEST(FileVersionInfoTest, GetVersionInfoFromNotExistingFile)
		{
			std::wstring fileName( L"" );

			ASSERT_THROW(FileVersionInfo::GetVersionInfo(fileName), std::exception);
		}
	}
}