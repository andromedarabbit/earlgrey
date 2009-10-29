#include "stdafx.h"

#include "Console.h"


namespace Earlgrey
{
	namespace Test
	{
		TEST(ConsoleTest, OpenFail)
		{
			Console console;
			ASSERT_FALSE(console.Open()); // GoogleTest already has a console.
		}
	}
}