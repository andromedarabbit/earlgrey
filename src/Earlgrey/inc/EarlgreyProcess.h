#pragma once
#include "Uncopyable.h"
#include "txstring.h"

namespace Earlgrey
{
	class Process : private Uncopyable
	{
	private:
		explicit Process();

	public:
		static _txstring MainModuleFileName();

		// static BOOL IsThis
		static DWORD GetParentProcessID(DWORD pid);
	};

}