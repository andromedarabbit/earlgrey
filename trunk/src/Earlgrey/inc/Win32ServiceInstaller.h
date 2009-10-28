#pragma once
// #include "Win32Service.h"
#include "Uncopyable.h"

namespace Earlgrey
{
	class Win32Service;

	class Win32ServiceInstaller : private Uncopyable
	{
	public:
		explicit Win32ServiceInstaller(Win32Service& service);

		BOOL InstallService();
		BOOL RemoveService();

	private:
		Win32Service& m_service;
	};
}