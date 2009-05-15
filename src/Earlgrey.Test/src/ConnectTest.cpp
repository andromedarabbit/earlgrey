#include "stdafx.h"

#include "ServerInit.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(ClientCreateTest, ClientCreate)
		{
			ServerInit();
			ServerCreate(100);
			ClientCreate(100);
		}
	}
}