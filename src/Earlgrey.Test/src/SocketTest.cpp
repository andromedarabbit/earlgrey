#include "stdafx.h"
#include "Socket.h"

#include "IPEndPoint.h"
#include "SocketTest.h"

namespace Earlgrey
{
 	namespace Test
 	{
		//TEST_F(SocketTest, CreateTcpSocket)
		//{
		//	Socket2 socket;
		//	ASSERT_TRUE(socket.CreateTcpSocket());
		//}

		//TEST_F(SocketTest, Bind)
		//{
		//	Socket socket;
		//	ASSERT_TRUE(socket.CreateTcpSocket());

		//	const USHORT port = 9879; // 설마 선점한 녀석은 없겠지?
		//	ASSERT_TRUE(socket.Bind(port));
		//}

 		TEST_F(SocketTest, BindNewVersion)
 		{
 			Socket socket;
 			// ASSERT_TRUE2(socket.CreateTcpSocket());
 
 			const USHORT port = 6849; // 설마 선점한 녀석은 없겠지?
 			IPEndPoint localEp(IPAddress2::Any(), port);
 
 			socket.Bind(localEp);
 		}

	}
}