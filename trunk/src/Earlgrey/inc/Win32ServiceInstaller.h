#pragma once
// #include "Win32Service.h"
#include "Uncopyable.h"
#include "txstring.h"

namespace Earlgrey
{
	class Win32Service;

	class Win32ServiceInstaller : private Uncopyable
	{
		const static TCHAR * REGISTRY_SERVICE_ROOT;
	public:
		explicit Win32ServiceInstaller(Win32Service& service);

		BOOL InstallService();
		BOOL RemoveService();

		void Description(const _txstring& description);		
		_txstring Description();
		const _txstring& Description() const;

	private:
		Win32Service& m_service;
		_txstring m_description;

	};
}