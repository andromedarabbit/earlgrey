// Earlgrey.Test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "AppInitializer.h"

int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	Earlgrey::AppInfo app;
	app.InitInstance(Earlgrey::AppType::E_APPTYPE_DEFAULT);

	return RUN_ALL_TESTS();
}

