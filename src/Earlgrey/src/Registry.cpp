#include "stdafx.h"
#include "Registry.h"
#include "Log.h"
#include "RAII.h"

namespace Earlgrey
{
	HKEY Registry::GetKey(const TCHAR * const keyPath, DWORD openOption)
	{
		HKEY hKey = 0;
		// Earlgrey::handle_t regKeyHandle(hKey, &RegCloseKey);

		LONG errCode = ::RegOpenKeyEx(
			HKEY_LOCAL_MACHINE
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

	BOOL Registry::KeyExists(const TCHAR * const keyPath)
	{
		Earlgrey::handle_t hKey(GetKey(keyPath), &RegCloseKey);

		if(hKey == NULL)
		{
			return FALSE;
		}
		return TRUE;
	}

	BOOL Registry::GetSubKeyNames(HKEY rootKey, xvector<_txstring>::Type& subKeys)
	{
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

		if (cSubKeys)
		{
			// printf( "\nNumber of subkeys: %d\n", cSubKeys);
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

				// _tprintf(TEXT("(%d) %s\n"), i+1, achKey);
				subKeys.push_back(achKey);
			}
		} 

		return TRUE;
		// Enumerate the key values. 
		/*
		TCHAR achValue[MAX_VALUE_NAME]; 
		DWORD cchValue = MAX_VALUE_NAME; 

		if (cValues) 
		{
		printf( "\nNumber of values: %d\n", cValues);

		for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) 
		{ 
		cchValue = MAX_VALUE_NAME; 
		achValue[0] = '\0'; 
		retCode = RegEnumValue(rootKey, i, 
		achValue, 
		&cchValue, 
		NULL, 
		NULL,
		NULL,
		NULL);

		if (retCode == ERROR_SUCCESS ) 
		{ 
		_tprintf(TEXT("(%d) %s\n"), i+1, achValue); 
		} 
		}
		}
		*/
	}


	BOOL Registry::GetSubKeyNames(const TCHAR * const rootKeyPath, xvector<_txstring>::Type& subKeys)
	{
		HKEY rootKey = GetKey(rootKeyPath);
		Earlgrey::handle_t hKey(rootKey, &RegCloseKey);
		return GetSubKeyNames(rootKey, subKeys);
	}

	BOOL Registry::DeleteKey(const TCHAR * const keyPath)
	{
		// HKEY key = GetKey(keyPath, KEY_ALL_ACCESS);
		// Earlgrey::handle_t hKey(key, &RegCloseKey);

		LONG errCode = ::RegDeleteTree(HKEY_LOCAL_MACHINE, keyPath);
		if(errCode != ERROR_SUCCESS)
		{
			// TODO
			return FALSE;
		}
		return TRUE;
	}
}