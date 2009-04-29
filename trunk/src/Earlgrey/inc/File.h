#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"

namespace Earlgrey
{
	class File : private Uncopyable
	{
	private:
		explicit File();

	public:
		static BOOL Exists(const _txstring& path);

	};
}