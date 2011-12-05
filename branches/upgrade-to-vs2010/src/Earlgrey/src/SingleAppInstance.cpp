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

		if(m_thisAppName == appName) // ���� �������α׷��� �� �޼��带 �� �� �̻� ȣ������ ��
			return TRUE;

		if(m_thisAppName.length() > 0 && m_thisAppName != appName) // �ٸ� �̸����� �� ����Ϸ��� �ϸ�
			return FALSE;


		HANDLE mutex = CreateMutex(NULL, FALSE, appName);
		const DWORD err = GetLastError();
		EARLGREY_VERIFY(mutex != NULL);

		if( err == ERROR_ALREADY_EXISTS ) // �ٸ� �������α׷��� �� �̸����� ��������� ���з� ����
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