#pragma once 
#include "Uncopyable.h"
#include "xvector.h"
#include "txstring.h"

namespace Earlgrey
{
	class RegistryKey;

	class Registry : private Uncopyable
	{
	private:
		explicit Registry();

	public:
		static RegistryKey ClassesRoot();
		static RegistryKey CurrentConfig();
		static RegistryKey CurrentUser();
		static RegistryKey LocalMachine();
		static RegistryKey Users();
	};
}