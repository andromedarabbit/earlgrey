#include "stdafx.h"
#include "Listener.h"

#include "SocketTest.h"

namespace Earlgrey
{
	namespace Test
	{
		class ListenerTest : public SocketTest {
		};

		TEST_F(ListenerTest, Listen)
		{
			const USHORT port = 9879; // 설마 선점한 녀석은 없겠지?
			
			Listener listener;
			ASSERT_TRUE(listener.Listen(port, true));
		}
	}
}
