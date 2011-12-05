#include "stdafx.h"
#include "SingleAppInstance.h"
#include "EarlgreyAssert.h"


namespace Earlgrey
{
	SingleAppInstance::SingleAppInstance()
		: m_thisAppName()
		, m_thisAppMutex(NULL)
	{

	}

	SingleAppInstance::~SingleAppInstance()
	{
		EARLGREY_VERIFY(::CloseHandle(m_thisAppMutex));
	}

	BOOL SingleAppInstance::RegisterThisApp(const TCHAR * appName)
	{
		EARLGREY_ASSERT(appName != NULL);
		EARLGREY_ASSERT(_tcslen(appName) > 0);

		if(m_thisAppName == appName) // 같은 응용프로그램이 이 메서드를 두 번 이상 호출했을 때
			return TRUE;

		if(m_thisAppName.length() > 0 && m_thisAppName != appName) // 다른 이름으로 또 등록하려고 하면
			return FALSE;


		HANDLE mutex = CreateMutex(NULL, FALSE, appName);
		const DWORD err = GetLastError();
		EARLGREY_VERIFY(mutex != NULL);

		if( err == ERROR_ALREADY_EXISTS ) // 다른 응용프로그램이 이 이름으로 등록했으면 실패로 간주
		{
			EARLGREY_VERIFY(::CloseHandle(mutex));
			return FALSE;
		}
		
		EARLGREY_ASSERT(m_thisAppName.empty());
		EARLGREY_ASSERT(m_thisAppMutex == NULL);
		m_thisAppName = appName;
		m_thisAppMutex = mutex;
	
		return TRUE;
	}

}