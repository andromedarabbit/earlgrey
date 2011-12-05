#pragma once
#include "DefaultAppSettings.h"

namespace Earlgrey
{
	namespace ServiceProcess
	{
		class ServiceSettings : public DefaultAppSettings
		{
		public:
			virtual BOOL AllowOnlyOneInstance() const;
		};
	}
}