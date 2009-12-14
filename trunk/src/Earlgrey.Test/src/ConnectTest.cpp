#include "stdafx.h"

#include "ServerInit.h"
#include "Application.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(ClientCreateTest, ClientCreate)
		{
			//AppInfo app;
			//app.InitInstance(AppType::E_APPTYPE_DEFAULT);
			ServerCreate(100);
			ClientCreate(100);
		}
	}
}