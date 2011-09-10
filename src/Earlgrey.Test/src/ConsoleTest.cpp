#include "stdafx.h"

#include "Console.h"


namespace Earlgrey
{
	namespace Test
	{
		TEST(ConsoleTest, OpenFail)
		{
			ASSERT_FALSE2(gConsole::Instance().Open()); // GoogleTest already has a console.
		}
	}
}