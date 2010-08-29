#include "stdafx.h"
#include "WindowsFirewall.h"

#include "EarlgreyProcess.h"
#include "File.h"

#include <comdef.h>

namespace Earlgrey
{
	namespace Test
	{
		TEST(WindowsFirewallTest, TurnOffTurnOn)
		{
			WindowsFirewall firewall;
			firewall.Initialize();

			//E_NOTIMPL
//			HRESULT hr =  firewall.TurnOff();
// const _txstring msg = _com_error(hr).ErrorMessage();

			/*ASSERT_EQ(S_OK, firewall.TurnOff());
			ASSERT_FALSE(firewall.IsTurnedOn());*/

			ASSERT_EQ(S_OK, firewall.TurnOn());
			ASSERT_TRUE(firewall.IsTurnedOn());
		}

		TEST(WindowsFirewallTest, AddAppToExceptionList)
		{
			WindowsFirewall firewall;
			firewall.Initialize();


			_txstring thisModuleName( Process::MainModuleFileName().c_str() );
			ASSERT_TRUE(
				File::Exists(thisModuleName)
				);

			_txstring newFile(thisModuleName);
			newFile += _T(".unittest.exe");

			ASSERT_TRUE(
				File::Copy(thisModuleName, newFile, TRUE)
				);

			ASSERT_EQ(
				S_OK, firewall.AddAppToExceptionList(newFile.c_str())
				);
			ASSERT_TRUE(
				firewall.IsAppEnabled(newFile.c_str())
				);
		}
		
	}
}