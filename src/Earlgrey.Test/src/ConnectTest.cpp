#include "stdafx.h"

#include "ServerInit.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(ClientCreateTest, ClientCreate)
		{
			Init();
			ServerCreate(100);
			ClientCreate(100);
		}
	}
}