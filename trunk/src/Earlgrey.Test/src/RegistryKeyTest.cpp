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

			const WCHAR * keyPath = L"SOFTWARE\\EarlgreyTest";

			if( localMachine.KeyExists(keyPath) )
			{
				ASSERT_TRUE2(localMachine.DeleteSubKeyTree(keyPath));
			}

			ASSERT_TRUE2(localMachine.CreateSubKey(keyPath));
			ASSERT_TRUE2(localMachine.DeleteSubKey(keyPath));
			
		}
	}
}