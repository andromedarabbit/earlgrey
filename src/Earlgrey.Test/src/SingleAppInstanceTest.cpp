#include "stdafx.h"
#include "SingleAppInstance.h"


namespace Earlgrey
{
	namespace Test
	{
		TEST(SingleAppInstanceTest, IsRunning)
		{
			const TCHAR * appName = _T("EarlgreyTest");
			
			ASSERT_FALSE(gSingleInstance::Instance().IsRunning(appName));
			ASSERT_TRUE(gSingleInstance::Instance().IsRunning(appName));
			ASSERT_TRUE(gSingleInstance::Instance().IsRunning(appName));
			ASSERT_TRUE(gSingleInstance::Instance().IsRunning(appName));
		}
	}
}