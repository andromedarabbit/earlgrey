#include "stdafx.h"
#include "SingleAppInstance.h"


namespace Earlgrey
{
	namespace Test
	{
		TEST(SingleAppInstanceTest, IsRunning)
		{
			const WCHAR * appName = L"Earlgrey.Test.exe";
			
			ASSERT_TRUE2(gSingleAppInstance::Instance().RegisterThisApp(appName));
			ASSERT_TRUE2(gSingleAppInstance::Instance().RegisterThisApp(appName));
			
			const WCHAR * anotherAppName = L"Another";
			ASSERT_FALSE2(gSingleAppInstance::Instance().RegisterThisApp(anotherAppName));
		}
	}
}