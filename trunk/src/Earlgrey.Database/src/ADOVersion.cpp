#include "stdafx.h"
#include "ADOVersion.h"

#include "Registry.h"
#include "lexical_cast.hpp"

namespace Earlgrey
{
	namespace ADO
	{
		_txstring GetMDACVersion()
		{			
			// TODO: ���߿� registry Ŭ������ ����� ��ȭ�ؼ� �ڵ带 �ٲ� �־�� �Ѵ�.
			HKEY hKey;
			LONG lRet;

			lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE, TEXT("Software\\Microsoft\\DataAccess"), 0, KEY_QUERY_VALUE, &hKey );
			if( lRet != ERROR_SUCCESS ) 
				return FALSE;

			const DWORD BUFSIZE = MAX_PATH;
			TCHAR szFullInstallVer[MAX_PATH];
			DWORD dwBufLen = BUFSIZE * sizeof(TCHAR);

			lRet = RegQueryValueEx( hKey, TEXT("FullInstallVer"), NULL, NULL, (LPBYTE) szFullInstallVer, &dwBufLen);
			RegCloseKey( hKey );

			if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE*sizeof(TCHAR)) )
				return FALSE;

			return _txstring(szFullInstallVer);
		}
	}
}