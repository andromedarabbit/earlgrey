// Earlgrey.Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Application.h"
#include "DefaultAppSettings.h"

using namespace Earlgrey;


int RunTests()
{
	return RUN_ALL_TESTS();
}

int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	DefaultAppSettings appSettings;

	Application app( appSettings );
	app.InitInstance();

	return app.RunOnce(RunTests);
}

