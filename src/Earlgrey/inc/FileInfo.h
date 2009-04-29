#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"

namespace Earlgrey
{
	class Path : private Uncopyable
	{
	private:
		explicit Path();

	public:
		static BOOL Exists(const _txstring& path);

		static _txstring GetDirectoryName(const _txstring& fullPath, BOOL strict);
		static _txstring GetDirectoryName(const _txstring& fullPath);

	};
}