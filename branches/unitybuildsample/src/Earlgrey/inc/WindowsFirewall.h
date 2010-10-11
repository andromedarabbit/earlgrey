#pragma once

#include "COMHelper.h"

#include <crtdbg.h>
#include <netfw.h>
#include <objbase.h>
#include <oleauto.h>

#include <atlbase.h> // CComPtr
struct INetFwProfile;
struct INetFwMgr;
struct INetFwPolicy;

namespace Earlgrey
{
	class WindowsFirewall : private Uncopyable
	{
	public:
		typedef LONG PORT_NO_TYPE;

		explicit WindowsFirewall(void);
		~WindowsFirewall(void);
		
		HRESULT Initialize(); 

		BOOL IsTurnedOn() const;

		HRESULT TurnOn();
		HRESULT TurnOff();

		BOOL IsAppEnabled() const;
		BOOL IsAppEnabled(IN const wchar_t* fwProcessImageFileName) const;
		
		HRESULT AddAppToExceptionList();
		HRESULT AddAppToExceptionList(IN const wchar_t* fwProcessImageFileName);
		HRESULT AddAppToExceptionList(IN const wchar_t* fwProcessImageFileName, IN const wchar_t* fwName);

		BOOL IsPortEnabled(IN PORT_NO_TYPE portNumber, IN NET_FW_IP_PROTOCOL ipProtocol);

		HRESULT AddPortToExceptionList(IN PORT_NO_TYPE portNumber, IN NET_FW_IP_PROTOCOL ipProtocol, IN const wchar_t* name);

	private:
		INetFwProfile* m_fwProfile;
	};
}