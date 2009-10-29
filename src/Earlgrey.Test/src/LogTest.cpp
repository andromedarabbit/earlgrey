#include "stdafx.h"

#include "Log.h"
#include "StringHelper.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(LogTest, ErrorMessageTest)
		{
			const DWORD errCode = 5;
			_txstring msg = Log::ErrorMessage(errCode);
			
			ASSERT_GT(msg.length(), 6);

			if(GetSystemDefaultLCID() == 1042) // �ѱ���
			{
				std::wcout <<_T("�׼����� �źεǾ����ϴ�.") <<  msg.c_str() <<std::endl;
				ASSERT_TRUE(
					StringHelper::StartsWith(msg.c_str(), _T("�׼����� �źεǾ����ϴ�."))					
					);
			}
		}
	}
}