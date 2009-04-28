#include "stdafx.h"
#include "SingleInstance.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	BOOL SingleAppInstance::IsRunning(AppType::E_Type appType)
	{
		EARLGREY_VERIFY(m_ExecutedOnce == FALSE);

		m_ExecutedOnce = TRUE;

		if(m_Mutex == NULL)
		{
			m_Mutex = CreateMutex(NULL, TRUE, AppType::Names[appType]);
			if(m_Mutex == NULL)
				return FALSE;

			if( GetLastError() == ERROR_ALREADY_EXISTS )
			{
				ReleaseMutex(m_Mutex);
				return FALSE;
			}
			return TRUE;
		}
		return TRUE;
	}
}