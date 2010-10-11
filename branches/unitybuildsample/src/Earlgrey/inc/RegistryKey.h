#pragma once 
#include "Uncopyable.h"
#include "xvector.h"
#include "txstring.h"

namespace Earlgrey
{
	class RegistryKey : private Uncopyable
	{
		friend class Registry;
	private:
		explicit RegistryKey(HKEY rootKey);

	public:
		HKEY GetKey(const TCHAR * const keyPath, DWORD openOption = KEY_READ);

		BOOL KeyExists(const TCHAR * const keyPath);

		BOOL GetSubKeyNames(HKEY rootKey, xvector<_txstring>::Type& subKeys);

		BOOL GetSubKeyNames(const TCHAR * const keyPath, xvector<_txstring>::Type& subKeys);

		BOOL CreateSubKey(const TCHAR * const keyPath);

		BOOL DeleteSubKey(const TCHAR * const keyPath);
		BOOL DeleteSubKeyTree(const TCHAR * const keyPath);

	private:
		HKEY m_rootKey;
	};
}