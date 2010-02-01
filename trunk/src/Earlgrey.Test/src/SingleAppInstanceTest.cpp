#include "stdafx.h"
#include "SingleAppInstance.h"


namespace Earlgrey
{
	namespace Test
	{
		TEST(SingleAppInstanceTest, IsRunning)
		{
			const TCHAR * appName = _T("Earlgrey.Test.exe");
			
			ASSERT_TRUE(gSingleAppInstance::Instance().RegisterThisApp(appName));
			ASSERT_TRUE(gSingleAppInstance::Instance().RegisterThisApp(appName));
			
			const TCHAR * anotherAppName = _T("Another");
			ASSERT_FALSE(gSingleAppInstance::Instance().RegisterThisApp(anotherAppName));
		}
	}
}