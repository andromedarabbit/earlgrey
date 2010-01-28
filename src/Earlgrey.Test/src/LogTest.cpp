#include "stdafx.h"

#include "Log.h"
#include "StringHelper.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(LogTest, ErrorMessageTest)
		{
			using namespace Earlgrey::String;

			const DWORD errCode = 5;
			_txstring msg = Log::ErrorMessage(errCode);
			
			ASSERT_GT(msg.length(), static_cast<size_t>(6));

			if(GetSystemDefaultLCID() == 1042) // 한국어
			{
				std::wcout <<_T("액세스가 거부되었습니다.") <<  msg.c_str() <<std::endl;
				ASSERT_TRUE(
					StartsWith(msg.c_str(), _T("액세스가 거부되었습니다."))					
					);
			}
		}
	}
}