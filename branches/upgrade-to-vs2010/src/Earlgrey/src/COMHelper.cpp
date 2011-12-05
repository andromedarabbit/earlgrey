#include "stdafx.h"
#include "COMHelper.h"

#include <objbase.h>

namespace Earlgrey
{
	BOOL COMHelper::s_Initialized = FALSE;

	BOOL COMHelper::Initialize()
	{
		if(s_Initialized == TRUE)
			return TRUE;

		const HRESULT hr = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
		if(hr != S_OK) // S_OK, S_FALSE, RPC_E_CHANGED_MODE
		{
			return FALSE;
		}

		s_Initialized = TRUE;
		return TRUE;
	}

	BOOL COMHelper::Uninitialize()
	{
		if(s_Initialized == FALSE)
			return TRUE;

		::CoUninitialize();
		return TRUE;
	}

	BOOL COMHelper::Initialized()
	{
		return s_Initialized;
	}
}
