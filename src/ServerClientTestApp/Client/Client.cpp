// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Windows.h>
#include "ServerInit.h"

int _tmain(int argc, _TCHAR* argv[])
{
	Earlgrey::ServerInit();
	Earlgrey::ServerCreate(100);

	Earlgrey::ClientCreate(100);

	return EXIT_SUCCESS;
}
