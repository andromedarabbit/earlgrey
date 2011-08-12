#include "stdafx.h"
#include "RegistryKey.h"
#include "Registry.h"

namespace Earlgrey
{
	namespace Test
	{
		TEST(RegistryKeyTest, ExpectTrue)
		{
			RegistryKey localMachine( Registry::LocalMachine() );

			const TCHAR * keyPath = _T("SOFTWARE\\EarlgreyTest");

			if( localMachine.KeyExists(keyPath) )
			{
				ASSERT_TRUE(localMachine.DeleteSubKeyTree(keyPath));
			}

			ASSERT_TRUE(localMachine.CreateSubKey(keyPath));
			ASSERT_TRUE(localMachine.DeleteSubKey(keyPath));
			
		}
	}
}