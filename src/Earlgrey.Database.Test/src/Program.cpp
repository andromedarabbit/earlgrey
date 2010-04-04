// Earlgrey.Test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Application.h"
#include "DatabaseTestAppSettings.h"

int _tmain(int argc, _TCHAR* argv[])
{
	testing::InitGoogleTest(&argc, argv);

	Earlgrey::Application app( 
		Earlgrey::DatabaseTestAppSettingsSingleton::Instance() 
		);
	app.InitInstance();

	return RUN_ALL_TESTS();
}
