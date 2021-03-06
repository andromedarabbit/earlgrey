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
			ASSERT_FALSE2(firewall.IsTurnedOn());*/

			ASSERT_EQ(S_OK, firewall.TurnOn());
			ASSERT_TRUE2(firewall.IsTurnedOn());
		}

		TEST(WindowsFirewallTest, AddAppToExceptionList)
		{
			WindowsFirewall firewall;
			firewall.Initialize();


			xwstring thisModuleName( Process::MainModuleFileNameW().c_str() );
			ASSERT_TRUE2(
				File::Exists(thisModuleName)
				);

			xwstring newFile(thisModuleName);
			newFile += L".unittest.exe";

			ASSERT_TRUE2(
				File::Copy(thisModuleName, newFile, TRUE)
				);

			ASSERT_EQ(
				S_OK, firewall.AddAppToExceptionList(newFile.c_str())
				);
			ASSERT_TRUE2(
				firewall.IsAppEnabled(newFile.c_str())
				);
		}
		
	}
}