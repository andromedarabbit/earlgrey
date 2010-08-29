#include "StdAfx.h"
#include "WindowsFirewall.h"
//
//#include <crtdbg.h>
//#include <netfw.h>
//#include <objbase.h>
//#include <oleauto.h>
// #include "tstring.h"
#include "Path.h"
#include "EarlgreyProcess.h"
#include "Environment.h"

namespace Earlgrey
{

	WindowsFirewall::WindowsFirewall()
		: m_fwProfile(NULL)
	{

	}

	WindowsFirewall::~WindowsFirewall()
	{

	}

	HRESULT WindowsFirewall::Initialize() // (OUT INetFwProfile*& m_fwProfile)
	{
		EARLGREY_ASSERT(COMHelper::Initialize());

		HRESULT hr = S_OK;
		CComPtr<INetFwMgr> fwMgr = NULL;
		CComPtr<INetFwPolicy> fwPolicy = NULL;

		EARLGREY_ASSERT(&m_fwProfile != NULL);

		m_fwProfile = NULL;

		// Create an instance of the firewall settings manager.
		hr = CoCreateInstance(
			__uuidof(NetFwMgr),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(INetFwMgr),
			(void**)&fwMgr
			);
		if (FAILED(hr))
		{
			printf("CoCreateInstance failed: 0x%08lx\n", hr);
			return hr;
		}

		// Retrieve the local firewall policy.
		hr = fwMgr->get_LocalPolicy(&fwPolicy);
		if (FAILED(hr))
		{
			printf("get_LocalPolicy failed: 0x%08lx\n", hr);
			return hr;
		}

		// Retrieve the firewall profile currently in effect.
		hr = fwPolicy->get_CurrentProfile(&m_fwProfile);
		if (FAILED(hr))
		{
			printf("get_CurrentProfile failed: 0x%08lx\n", hr);
			return hr;
		}

		return hr;
	}

	BOOL WindowsFirewall::IsTurnedOn() const
	{
		EARLGREY_ASSERT(m_fwProfile != NULL);

		// Get the current state of the firewall.
		VARIANT_BOOL fwEnabled;
		HRESULT hr = m_fwProfile->get_FirewallEnabled(&fwEnabled);
		if (FAILED(hr))
		{
			printf("get_FirewallEnabled failed: 0x%08lx\n", hr);
			throw std::exception();
		}

		// Check to see if the firewall is on.
		return fwEnabled != VARIANT_FALSE;
	}

	HRESULT WindowsFirewall::TurnOn()
	{
		EARLGREY_ASSERT(m_fwProfile != NULL);

		// Check to see if the firewall is off.
		if (IsTurnedOn() == TRUE)
			return S_OK; // 이미 작동 중이니 성공으로 보자.

		// Turn the firewall on.
		return m_fwProfile->put_FirewallEnabled(VARIANT_TRUE);
	}

	HRESULT WindowsFirewall::TurnOff()
	{
		EARLGREY_ASSERT(m_fwProfile != NULL);

		_txstring osVersion( Environment::OSVersion() );
DBG_UNREFERENCED_LOCAL_VARIABLE(osVersion);
		// Check to see if the firewall is off.
		if (IsTurnedOn() == FALSE)
			return S_OK; 

		// Turn the firewall on.
		return m_fwProfile->put_FirewallEnabled(VARIANT_FALSE);

	}


	BOOL WindowsFirewall::IsAppEnabled() const
	{
		_tstring thisModuleName(
			Process::MainModuleFileName()
			);
		return IsAppEnabled(thisModuleName.c_str());
	}

	BOOL WindowsFirewall::IsAppEnabled(IN const wchar_t* fwProcessImageFileName) const
	{
		EARLGREY_ASSERT(m_fwProfile != NULL);
		EARLGREY_ASSERT(fwProcessImageFileName != NULL);
				
		CComPtr<INetFwAuthorizedApplications> fwApps = NULL;
		
		// Retrieve the authorized application collection.
		HRESULT hr = m_fwProfile->get_AuthorizedApplications(&fwApps);
		if (FAILED(hr))
		{
			printf("get_AuthorizedApplications failed: 0x%08lx\n", hr);
			throw std::exception();
		}

		// Allocate a BSTR for the process image file name.
		BSTR fwBstrProcessImageFileName = SysAllocString(fwProcessImageFileName);
		if (fwBstrProcessImageFileName == NULL)
		{
			hr = E_OUTOFMEMORY;
			printf("SysAllocString failed: 0x%08lx\n", hr);
			throw std::exception();
		}

		// Attempt to retrieve the authorized application.
		CComPtr<INetFwAuthorizedApplication> fwApp = NULL;
		hr = fwApps->Item(fwBstrProcessImageFileName, &fwApp);
		
		// Free the BSTR.
		SysFreeString(fwBstrProcessImageFileName);

		if (FAILED(hr))
			return FALSE;

		// Find out if the authorized application is enabled.
		VARIANT_BOOL fwEnabled;
		hr = fwApp->get_Enabled(&fwEnabled);
		if (FAILED(hr))
		{
			printf("get_Enabled failed: 0x%08lx\n", hr);
			throw std::exception();
		}

		return fwEnabled != VARIANT_FALSE;				
	}

	HRESULT WindowsFirewall::AddAppToExceptionList()
	{
		_tstring thisModuleName(
			Process::MainModuleFileName()
			);

		return AddAppToExceptionList(thisModuleName.c_str());
	}

	HRESULT WindowsFirewall::AddAppToExceptionList(IN const wchar_t* fwProcessImageFileName)
	{
		_tstring name(
			Path::GetFileName(fwProcessImageFileName)
			);
		return AddAppToExceptionList(fwProcessImageFileName, name.c_str());
	}

	HRESULT WindowsFirewall::AddAppToExceptionList(IN const wchar_t* fwProcessImageFileName, IN const wchar_t* fwName)
	{
		EARLGREY_ASSERT(m_fwProfile != NULL);
		EARLGREY_ASSERT(fwProcessImageFileName != NULL);
		EARLGREY_ASSERT(fwName != NULL);

		if(IsAppEnabled(fwProcessImageFileName) == TRUE)
			return S_OK;


		// Retrieve the authorized application collection.		
		CComPtr<INetFwAuthorizedApplications> fwApps = NULL;
		HRESULT hr = m_fwProfile->get_AuthorizedApplications(&fwApps);
		if (FAILED(hr))
		{
			printf("get_AuthorizedApplications failed: 0x%08lx\n", hr);
			return hr;
		}

		// Create an instance of an authorized application.
		CComPtr<INetFwAuthorizedApplication> fwApp = NULL;
		hr = CoCreateInstance(
			__uuidof(NetFwAuthorizedApplication),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(INetFwAuthorizedApplication),
			(void**)&fwApp
			);
		if (FAILED(hr))
		{
			printf("CoCreateInstance failed: 0x%08lx\n", hr);
			return hr;
		}

		// Allocate a BSTR for the process image file name.
		BSTR fwBstrProcessImageFileName = SysAllocString(fwProcessImageFileName);
		if (fwBstrProcessImageFileName == NULL)
		{
			hr = E_OUTOFMEMORY;
			printf("SysAllocString failed: 0x%08lx\n", hr);
			return hr;
		}

		// Set the process image file name.
		hr = fwApp->put_ProcessImageFileName(fwBstrProcessImageFileName);
		SysFreeString(fwBstrProcessImageFileName);
		if (FAILED(hr))
		{
			printf("put_ProcessImageFileName failed: 0x%08lx\n", hr);
			return hr;
		}

		// Allocate a BSTR for the application friendly name.
		BSTR fwBstrName = SysAllocString(fwName);
		if (SysStringLen(fwBstrName) == 0)
		{
			hr = E_OUTOFMEMORY;
			printf("SysAllocString failed: 0x%08lx\n", hr);
			return hr;
		}

		// Set the application friendly name.
		hr = fwApp->put_Name(fwBstrName);
		SysFreeString(fwBstrName);

		if (FAILED(hr))
		{
			printf("put_Name failed: 0x%08lx\n", hr);
			return hr;
		}

		// Add the application to the collection.
		hr = fwApps->Add(fwApp);
		if (FAILED(hr))
		{
			printf("Add failed: 0x%08lx\n", hr);
			return hr;
		}

		return hr;
	}

	BOOL WindowsFirewall::IsPortEnabled(
		IN WindowsFirewall::PORT_NO_TYPE portNumber
		, IN NET_FW_IP_PROTOCOL ipProtocol
		)
	{
		EARLGREY_ASSERT(m_fwProfile != NULL);
		
		// Retrieve the globally open ports collection.
		CComPtr<INetFwOpenPorts> fwOpenPorts = NULL;
		HRESULT hr = m_fwProfile->get_GloballyOpenPorts(&fwOpenPorts);
		if (FAILED(hr))
		{
			printf("get_GloballyOpenPorts failed: 0x%08lx\n", hr);
			throw std::exception();
		}

		// Attempt to retrieve the globally open port.
		CComPtr<INetFwOpenPort> fwOpenPort = NULL;
		hr = fwOpenPorts->Item(portNumber, ipProtocol, &fwOpenPort);
		if (FAILED(hr))
			return FALSE;

		// Find out if the globally open port is enabled.
		VARIANT_BOOL fwEnabled;
		hr = fwOpenPort->get_Enabled(&fwEnabled);
		if (FAILED(hr))
		{
			printf("get_Enabled failed: 0x%08lx\n", hr);
			throw std::exception();
		}

		return fwEnabled != VARIANT_FALSE;
	}

	HRESULT WindowsFirewall::AddPortToExceptionList(
		IN WindowsFirewall::PORT_NO_TYPE portNumber
		, IN NET_FW_IP_PROTOCOL ipProtocol
		, IN const wchar_t* name
		)
	{
		EARLGREY_ASSERT(m_fwProfile != NULL);
		EARLGREY_ASSERT(name != NULL);


		// First check to see if the port is already added.
		if(IsPortEnabled(portNumber, ipProtocol))
			return S_OK;

		// Only add the port if it isn't already added.
		// Retrieve the collection of globally open ports.
		CComPtr<INetFwOpenPorts> fwOpenPorts = NULL;
		HRESULT hr = m_fwProfile->get_GloballyOpenPorts(&fwOpenPorts);
		if (FAILED(hr))
		{
			printf("get_GloballyOpenPorts failed: 0x%08lx\n", hr);
			return hr;
		}

		// Create an instance of an open port.
		CComPtr<INetFwOpenPort> fwOpenPort = NULL;
		hr = CoCreateInstance(
			__uuidof(NetFwOpenPort),
			NULL,
			CLSCTX_INPROC_SERVER,
			__uuidof(INetFwOpenPort),
			(void**)&fwOpenPort
			);
		if (FAILED(hr))
		{
			printf("CoCreateInstance failed: 0x%08lx\n", hr);
			return hr;
		}

		// Set the port number.
		hr = fwOpenPort->put_Port(portNumber);
		if (FAILED(hr))
		{
			printf("put_Port failed: 0x%08lx\n", hr);
			return hr;
		}

		// Set the IP protocol.
		hr = fwOpenPort->put_Protocol(ipProtocol);
		if (FAILED(hr))
		{
			printf("put_Protocol failed: 0x%08lx\n", hr);
			return hr;
		}

		// Allocate a BSTR for the friendly name of the port.
		BSTR fwBstrName = SysAllocString(name);
		if (SysStringLen(fwBstrName) == 0)
		{
			hr = E_OUTOFMEMORY;
			printf("SysAllocString failed: 0x%08lx\n", hr);
			return hr;
		}

		// Set the friendly name of the port.
		hr = fwOpenPort->put_Name(fwBstrName);
		SysFreeString(fwBstrName);
		if (FAILED(hr))
		{
			printf("put_Name failed: 0x%08lx\n", hr);
			return hr;
		}

		// Opens the port and adds it to the collection.
		hr = fwOpenPorts->Add(fwOpenPort);
		if (FAILED(hr))
		{
			printf("Add failed: 0x%08lx\n", hr);
			return hr;
		}
		
		return hr;
	}
}