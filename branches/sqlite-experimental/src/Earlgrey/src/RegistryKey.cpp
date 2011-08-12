#include "stdafx.h"
#include "RegistryKey.h"
#include "Log.h"
#include "RAII.h"

#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

namespace Earlgrey
{
	RegistryKey::RegistryKey(HKEY rootKey)
		: m_rootKey(rootKey)
	{
		EARLGREY_ASSERT(m_rootKey != NULL);
	}

	HKEY RegistryKey::GetKey(const TCHAR * const keyPath, DWORD openOption)
	{
		EARLGREY_ASSERT(keyPath != NULL);

		HKEY hKey = NULL;
		LONG errCode = ::RegOpenKeyEx(
			m_rootKey
			, keyPath
			, 0
			, openOption
			, &hKey
			);

		if(errCode != ERROR_SUCCESS)
		{
			// TODO
			_txstring msg = Log::ErrorMessage(errCode);
			DBG_UNREFERENCED_LOCAL_VARIABLE(msg);
			return NULL;
		}
		return hKey;
	}

	BOOL RegistryKey::KeyExists(const TCHAR * const keyPath)
	{
		EARLGREY_ASSERT(keyPath != NULL);

		Earlgrey::handle_t hKey(GetKey(keyPath), &RegCloseKey);
		if(hKey == NULL)
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL RegistryKey::GetSubKeyNames(HKEY rootKey, xvector<_txstring>::Type& subKeys)
	{
		EARLGREY_ASSERT(rootKey != NULL);
		EARLGREY_ASSERT(subKeys.size() == 0);

		const int MAX_VALUE_NAME = 16383;
		const int MAX_KEY_LENGTH = 255;

		TCHAR    achKey[MAX_VALUE_NAME];   // buffer for subkey name
		DWORD    cbName;                   // size of name string 
		TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
		DWORD    cchClassName = MAX_PATH;  // size of class string 
		DWORD    cSubKeys=0;               // number of subkeys 
		DWORD    cbMaxSubKey;              // longest subkey size 
		DWORD    cchMaxClass;              // longest class string 
		DWORD    cValues;              // number of values for key 
		DWORD    cchMaxValue;          // longest value name 
		DWORD    cbMaxValueData;       // longest value data 
		DWORD    cbSecurityDescriptor; // size of security descriptor 
		FILETIME ftLastWriteTime;      // last write time 

		// Get the class name and the value count. 
		DWORD retCode = RegQueryInfoKey(
			rootKey,                    // key handle 
			achClass,                // buffer for class name 
			&cchClassName,           // size of class string 
			NULL,                    // reserved 
			&cSubKeys,               // number of subkeys 
			&cbMaxSubKey,            // longest subkey size 
			&cchMaxClass,            // longest class string 
			&cValues,                // number of values for this key 
			&cchMaxValue,            // longest value name 
			&cbMaxValueData,         // longest value data 
			&cbSecurityDescriptor,   // security descriptor 
			&ftLastWriteTime);       // last write time 

		if(retCode != ERROR_SUCCESS)
		{
			// TODO
			return FALSE;
		}

		// Enumerate the subkeys, until RegEnumKeyEx fails.
		if (cSubKeys == 0)
			return TRUE;

		subKeys.reserve(cSubKeys);

		for (DWORD i=0; i<cSubKeys; i++) 
		{ 
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(rootKey, i,
				achKey, 
				&cbName, 
				NULL, 
				NULL, 
				NULL, 
				&ftLastWriteTime); 
			if (retCode != ERROR_SUCCESS) 
			{
				// TODO
			}

			subKeys.push_back(achKey);
		} 

		return TRUE;
	}


	BOOL RegistryKey::GetSubKeyNames(const TCHAR * const keyPath, xvector<_txstring>::Type& subKeys)
	{
		EARLGREY_ASSERT(keyPath != NULL);
		EARLGREY_ASSERT(subKeys.size() == 0);

		HKEY rootKey = GetKey(keyPath);
		Earlgrey::handle_t hKey(rootKey, &RegCloseKey);
		return GetSubKeyNames(rootKey, subKeys);
	}

	BOOL RegistryKey::CreateSubKey(const TCHAR * const keyPath)
	{
		EARLGREY_ASSERT(keyPath != NULL);

		HKEY hkey = NULL;
		DWORD disposition  = 0;

		LONG errCode = ::RegCreateKeyEx(
			m_rootKey
			, keyPath
			, 0
			, NULL
			, REG_OPTION_NON_VOLATILE
			, 0
			, NULL
			, &hkey
			, &disposition
			);
		if(errCode != ERROR_SUCCESS)
		{
			// TODO
			return FALSE;
		}
		return TRUE;
	}

	BOOL RegistryKey::DeleteSubKey(const TCHAR * const keyPath)
	{
		EARLGREY_ASSERT(keyPath != NULL);

		LONG errCode = ::RegDeleteKey(m_rootKey, keyPath);
		if(errCode != ERROR_SUCCESS)
		{
			// TODO
			return FALSE;
		}
		return TRUE;
	}

	BOOL RegistryKey::DeleteSubKeyTree(const TCHAR * const keyPath)
	{
		EARLGREY_ASSERT(keyPath != NULL);

		// LONG errCode = ::RegDeleteTree(m_rootKey, keyPath);
		LONG errCode = ::SHDeleteKey(m_rootKey, keyPath);
		if(errCode != ERROR_SUCCESS)
		{
			// TODO
			return FALSE;
		}
		return TRUE;
	}

}