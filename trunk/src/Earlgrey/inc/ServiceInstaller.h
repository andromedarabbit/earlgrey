#pragma once
#include "Uncopyable.h"
#include "txstring.h"
#include "RegistryKey.h"

namespace Earlgrey
{
	namespace ServiceProcess
	{
		class ServiceBase;
		// class RegistryKey;

		class ServiceInstaller : private Uncopyable
		{
			const static WCHAR * REGISTRY_SERVICE_ROOT;
		public:
			explicit ServiceInstaller(ServiceBase& service);

			BOOL InstallService();
			BOOL RemoveService();

			void Description(const xwstring& description);		
			xwstring Description();
			const xwstring& Description() const;

		private:
			ServiceBase& m_service;
			xwstring m_description;
			RegistryKey m_localMachineKey;

		};
	}
}