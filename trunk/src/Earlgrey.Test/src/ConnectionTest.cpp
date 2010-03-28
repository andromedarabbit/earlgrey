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
// 			Socket socket;
// 			ASSERT_TRUE(socket.CreateTcpSocket());

			const LPCSTR server = ("kaistizen.net");
			const INT port = 80;

			Connector connector;
			ASSERT_TRUE( connector.Connect(server, port) );

// 			Connection<DefaultReader, DefaultWriter>* connection = new Connection<DefaultReader, DefaultWriter>();
// 			connection->Initialize( socket );
		}
	}
}
