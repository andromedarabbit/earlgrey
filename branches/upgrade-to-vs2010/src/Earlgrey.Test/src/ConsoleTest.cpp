#include "stdafx.h"

#include "Console.h"


namespace Earlgrey
{
	namespace Test
	{
		TEST(ConsoleTest, OpenFail)
		{
			ASSERT_FALSE2(gConsole::Instance().Open(TRUE)); // GoogleTest already has a console.
			ASSERT_FALSE2(gConsole::Instance().Open(FALSE)); // GoogleTest already has a console.
		}

	}
}