// Earlgrey.Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Application.h"
#include "DefaultAppSettings.h"

using namespace Earlgrey;

int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	DefaultAppSettings appSettings;

	Application app(AppType::E_APPTYPE_DEFAULT, appSettings);
	app.InitInstance();

	return RUN_ALL_TESTS();
}

