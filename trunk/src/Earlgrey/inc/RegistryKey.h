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
		HKEY GetKey(const WCHAR * const keyPath, DWORD openOption = KEY_READ);

		BOOL KeyExists(const WCHAR * const keyPath);

		BOOL GetSubKeyNames(HKEY rootKey, xvector<xwstring>::Type& subKeys);

		BOOL GetSubKeyNames(const WCHAR * const keyPath, xvector<xwstring>::Type& subKeys);

		BOOL CreateSubKey(const WCHAR * const keyPath);

		BOOL DeleteSubKey(const WCHAR * const keyPath);
		BOOL DeleteSubKeyTree(const WCHAR * const keyPath);

	private:
		HKEY m_rootKey;
	};
}