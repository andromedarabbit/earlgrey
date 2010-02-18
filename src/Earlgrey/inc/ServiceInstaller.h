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
			const static TCHAR * REGISTRY_SERVICE_ROOT;
		public:
			explicit ServiceInstaller(ServiceBase& service);

			BOOL InstallService();
			BOOL RemoveService();

			void Description(const _txstring& description);		
			_txstring Description();
			const _txstring& Description() const;

		private:
			ServiceBase& m_service;
			_txstring m_description;
			RegistryKey m_localMachineKey;

		};
	}
}