// Earlgrey.Test.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "Application.h"
#include "DatabaseTestAppSettings.h"

// #include <boost/thread.hpp>


int _tmain(int argc, _TCHAR* argv[])
{
	// TODO 임시
	if(::CoInitializeEx(NULL, COINIT_MULTITHREADED) == S_FALSE) // S_OK, S_FALSE, RPC_E_CHANGED_MODE
	{
		// TODO: 오류처리
	}

	testing::InitGoogleTest(&argc, argv);

	Earlgrey::Application app( 
		Earlgrey::DatabaseTestAppSettingsSingleton::Instance() 
		);
	app.InitInstance();

	return RUN_ALL_TESTS();
}
