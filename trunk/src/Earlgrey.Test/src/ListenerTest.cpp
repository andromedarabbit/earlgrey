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
			const USHORT port = 9879; // ���� ������ �༮�� ������?
			
			Listener listener;
			ASSERT_TRUE(listener.Listen(port, true));
		}
	}
}
