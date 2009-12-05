#pragma once
#include "txstring.h"

namespace Earlgrey
{
	class Uncopyable;

	class File : private Uncopyable
	{
	private:
		explicit File();

	public:
		static BOOL Exists(const _txstring& path);
		static BOOL Delete(const _txstring& path);

	};
}