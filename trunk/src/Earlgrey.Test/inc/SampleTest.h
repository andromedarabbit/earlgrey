#pragma once

namespace Earlgrey
{
	namespace Test
	{
		TEST(SampleTest, ExpectTrue)
		{
			bool boolVar = true;
			EXPECT_TRUE(boolVar);
		}

		
	}
}