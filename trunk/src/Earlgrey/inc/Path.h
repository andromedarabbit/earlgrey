#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	class Path : private Uncopyable
	{
	private:
		explicit Path();

	public:
		static BOOL Exists(const _txstring& path);
		
		static _txstring GetDirectoryName(const _txstring& fullPath);

	};
}