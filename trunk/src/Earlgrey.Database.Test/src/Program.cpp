// Earlgrey.Test.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "Application.h"
#include "DatabaseTestAppSettings.h"

// #include <boost/thread.hpp>


int _tmain(int argc, _TCHAR* argv[])
{
	// TODO �ӽ�
	if(::CoInitializeEx(NULL, COINIT_MULTITHREADED) == S_FALSE) // S_OK, S_FALSE, RPC_E_CHANGED_MODE
	{
		// TODO: ����ó��
	}

	testing::InitGoogleTest(&argc, argv);

	Earlgrey::Application app( 
		Earlgrey::DatabaseTestAppSettingsSingleton::Instance() 
		);
	app.InitInstance();

	return RUN_ALL_TESTS();
}
