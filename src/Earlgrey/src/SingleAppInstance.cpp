#include "stdafx.h"
#include "SingleAppInstance.h"
#include "EarlgreyAssert.h"


namespace Earlgrey
{
	SingleAppInstance::~SingleAppInstance()
	{
		MutexCollectionType::iterator it = m_handles.begin();
		for( ; it != m_handles.end(); it++)
		{			
			EARLGREY_VERIFY(::CloseHandle(it->second));
		}
	}

	BOOL SingleAppInstance::IsRunning(const TCHAR * appName)
	{
		HANDLE m_Mutex = CreateMutex(NULL, FALSE, appName);
		const DWORD err = GetLastError();
		EARLGREY_VERIFY(m_Mutex != NULL);

		if( err == ERROR_ALREADY_EXISTS )
		{
			EARLGREY_VERIFY(::CloseHandle(m_Mutex));
			return TRUE;
		}
		
		m_handles[appName] = m_Mutex;

		return FALSE;
	}

	BOOL SingleAppInstance::IsRunning(AppType::E_Type appType)
	{
		return IsRunning(AppType::Names[appType]);
	}
}