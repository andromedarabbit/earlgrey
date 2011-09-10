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
			_tstring fileName( Process::MainModuleFileName() );

			FileVersionInfo versionInfo( FileVersionInfo::GetVersionInfo(fileName) );
			_tstring fileVersion = versionInfo.FileVersion();
			ASSERT_STREQ( _T("1.0.0.1"), fileVersion.c_str() );

			_tstring productName = versionInfo.ProductName();
			ASSERT_STREQ( _T("ProductName: Earlgrey"), productName.c_str() );

			_tstring fileDescription = versionInfo.FileDescription();
			ASSERT_STREQ( _T("FileDescription: Earlgrey"), fileDescription.c_str() );
			
		}

		TEST(FileVersionInfoTest, GetVersionInfoFromNotExistingFile)
		{
			_tstring fileName( _T("") );

			ASSERT_THROW(FileVersionInfo::GetVersionInfo(fileName), std::exception);
		}
	}
}