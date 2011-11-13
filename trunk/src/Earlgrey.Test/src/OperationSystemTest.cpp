#include "stdafx.h"
#include "OperatingSystem.h"


namespace Earlgrey
{
	namespace Test
	{	
		TEST(OperatingSystemTest, OSVersion)
		{		
			_txstring name = OperatingSystem::OSVersion();
			ASSERT_TRUE(name.length() > 0);
		}

		TEST(OperatingSystemTest, IsWinXpOrLater)
		{		
			ASSERT_TRUE(OperatingSystem::IsWinXp() || OperatingSystem::IsWinNt());
		}
	}
}
