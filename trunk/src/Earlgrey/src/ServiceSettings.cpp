#include "stdafx.h"
#include "ServiceSettings.h"
#include "ServiceHelper.h"


namespace Earlgrey
{
	namespace ServiceProcess
	{
		BOOL ServiceSettings::AllowOnlyOneInstance() const
		{
			if(Helper::RunByWin32Service())
				return TRUE;

			return FALSE;
		}
	}
}