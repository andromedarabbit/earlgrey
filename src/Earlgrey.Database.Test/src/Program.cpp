// Earlgrey.Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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
