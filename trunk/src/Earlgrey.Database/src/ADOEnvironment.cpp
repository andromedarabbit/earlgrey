#include "stdafx.h"
#include "ADOEnvironment.h"

#include "Registry.h"
#include "lexical_cast.hpp"

#include <objbase.h>

namespace Earlgrey
{
	namespace ADO
	{
		_txstring Environment::MDACVersion()
		{			
			// TODO: 나중에 registry 클래스의 기능을 강화해서 코드를 바꿔 넣어야 한다.
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

		BOOL Environment::s_Initialized = FALSE;
		// Environment::mutex_type Environment::s_InternalSyncObject;


		namespace 
		{
			BOOL StartupADO()
			{
				_txstring versionStr( Environment::MDACVersion() );
				if(versionStr.empty())
					return FALSE;

				size_t found = versionStr.find(_T("."));
				if(found != _txstring::npos)
				{
					found++;
					if(found < versionStr.size())
						versionStr = versionStr.substr(0, found + 1);
				}
				
				const float version = EARLGREY_LEXICAL_CAST<float>(versionStr);

				if(version < 2.8f) // TODO: 임시로 최소 지원 버전을 2.8 로 설정한다.
					return FALSE;

				if(::CoInitializeEx(NULL, COINIT_MULTITHREADED) != S_OK) // S_OK, S_FALSE, RPC_E_CHANGED_MODE
				{
					return FALSE;
				}

				return TRUE;
			}

			void CleanupADO()
			{
				::CoUninitialize();
			}
		}

		void Environment::InitializeADO()
		{
			if(s_Initialized)
				return;

			// 		scoped_lock_type scopeLock(s_InternalSyncObject);
			// 
			// 		if(s_Initialized)
			// 			return;

			if(StartupADO())
				s_Initialized = TRUE;
		}

		void Environment::UninitializeADO()
		{
			if(s_Initialized == FALSE)
				return;

			CleanupADO();
			s_Initialized = FALSE;
		}
	}
}