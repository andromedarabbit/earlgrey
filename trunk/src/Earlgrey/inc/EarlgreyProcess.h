#pragma once
#include "txstring.h"

namespace Earlgrey
{
	class Uncopyable;

	class Process : private Uncopyable
	{
	private:
		explicit Process();

	public:
		static _txstring MainModuleFileName();

		// static BOOL IsThis
		static DWORD GetParentProcessID(DWORD pid);
		static _txstring GetParentProcessName(DWORD pid);

	};

}