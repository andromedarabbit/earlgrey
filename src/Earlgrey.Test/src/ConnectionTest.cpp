#include "stdafx.h"
#include "Connection.h"

#include "Connector.h"

#include "SocketTest.h"

namespace Earlgrey
{
	namespace Test
	{
		class ConnectionTest : public SocketTest {
		};

		TEST_F(ConnectionTest, Listen)
		{
			/*const LPCSTR server = ("kaistizen.net");
			const INT port = 80;

			Connector connector;
			*/


			// \todo WaitThread에서 Connector를 참조하기 때문에 연결되기 전에 Connector를 제거하면 안된다.
			// 나중에 수정해야 함
			//ASSERT_TRUE( connector.Connect(server, port) );
		}
	}
}
