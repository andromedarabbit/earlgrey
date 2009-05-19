#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"

namespace Earlgrey 
{ 
	class Environment : private Uncopyable
	{
	private:
		explicit Environment();

	public:
		static const TCHAR* const NewLine();

		static DWORD GetProcessorCount();

		static _txstring BaseDirectory();
	};
}