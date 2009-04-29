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
		static _txstring GetDirectoryName(const _txstring& fullPath)
		{
			EARLGREY_ASSERT(fullPath.length() > 0);
			if(fullPath[fullPath.length() - 1] != TEXT('\\'))
				return fullPath;

			std::size_t found = fullPath.rfind(TEXT('\\'));
			if (found != _txstring::npos)
			{
				return fullPath.substr(0, found);
			}
			return fullPath;
		}

	};
}