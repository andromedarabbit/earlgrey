// Earlgrey.Test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

