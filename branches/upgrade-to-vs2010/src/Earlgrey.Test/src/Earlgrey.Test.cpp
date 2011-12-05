// Earlgrey.Test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

