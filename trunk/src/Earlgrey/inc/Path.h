#pragma once
#include "txstring.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	class Uncopyable;

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

	public: // Fields
		static const TCHAR AltDirectorySeparatorChar = '/';
		static const TCHAR DirectorySeparatorChar = '\\';
		static const TCHAR VolumeSeparatorChar = ':';



	};
}