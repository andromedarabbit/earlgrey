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
			ASSERT_TRUE( fileVersion == _T("1.0.0.1") );

			_tstring productName = versionInfo.ProductName();
			ASSERT_TRUE( productName ==  _T("ProductName: Earlgrey") );

			_tstring fileDescription = versionInfo.FileDescription();
			ASSERT_TRUE( fileDescription ==  _T("FileDescription: Earlgrey") );
			
		}

		TEST(FileVersionInfoTest, GetVersionInfoFromNotExistingFile)
		{
			_tstring fileName( _T("") );

			ASSERT_THROW(FileVersionInfo::GetVersionInfo(fileName), std::exception);
		}
	}
}