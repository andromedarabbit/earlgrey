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


			// \todo WaitThread���� Connector�� �����ϱ� ������ ����Ǳ� ���� Connector�� �����ϸ� �ȵȴ�.
			// ���߿� �����ؾ� ��
			//ASSERT_TRUE( connector.Connect(server, port) );
		}
	}
}
