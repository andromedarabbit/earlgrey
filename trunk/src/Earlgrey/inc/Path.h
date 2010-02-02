#pragma once
#include "Uncopyable.h"
#include "tstring.h"
#include "txstring.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	class Path : private Uncopyable
	{
	private:
		explicit Path();

	public:
		static BOOL IsPathRooted(const _txstring& path);
		static _txstring Combine(const _txstring& path1, const _txstring& path2);
		static BOOL Exists(const _txstring& path);
		
		static _txstring GetDirectoryName(const _txstring& fullPath, BOOL strict);
		static _txstring GetDirectoryName(const _txstring& fullPath);

		static BOOL IsDirectorySeparator(TCHAR c);

		static _tstring GetFileName(const _tstring& path);
#ifndef EARLGREY_BUILD_STL_ALLOCATOR
		static _txstring GetFileName(const _txstring& path);
#endif
		
		static _txstring ResolveFilePath(const _txstring& fileName, BOOL throwNotFoundException = FALSE);

		
	public: // Fields
		static const TCHAR AltDirectorySeparatorChar = '/';
		static const TCHAR DirectorySeparatorChar = '\\';
		static const TCHAR VolumeSeparatorChar = ':';
	};
}