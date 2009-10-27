#pragma once 
#include "Uncopyable.h"
#include "xvector.h"
#include "txstring.h"

namespace Earlgrey
{
	//! \todo supports other than LOCAL_MACHINE
	class Registry : private Uncopyable
	{
	private:
		explicit Registry();

	public:
		static HKEY GetKey(const TCHAR * const keyPath, DWORD openOption = KEY_READ);

		static BOOL KeyExists(const TCHAR * const keyPath);

		static BOOL GetSubKeyNames(HKEY rootKey, xvector<_txstring>::Type& subKeys);

		static BOOL GetSubKeyNames(const TCHAR * const rootKeyPath, xvector<_txstring>::Type& subKeys);

		static BOOL DeleteKey(const TCHAR * const keyPath);
	};
}