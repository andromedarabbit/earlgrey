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
			_txstring fileName( Process::MainModuleFileName() );

			FileVersionInfo versionInfo( FileVersionInfo::GetVersionInfo(fileName) );
			_txstring fileVersion = versionInfo.FileVersion();
			ASSERT_TRUE( fileVersion == _T("1.0.0.1") );

			_txstring productName = versionInfo.ProductName();
			ASSERT_TRUE( productName ==  _T("ProductName: Earlgrey") );

			_txstring fileDescription = versionInfo.FileDescription();
			ASSERT_TRUE( fileDescription ==  _T("FileDescription: Earlgrey") );
			
		}

		TEST(FileVersionInfoTest, GetVersionInfoFromNotExistingFile)
		{
			_txstring fileName( _T("") );

			ASSERT_THROW(FileVersionInfo::GetVersionInfo(fileName), std::exception);
		}
	}
}