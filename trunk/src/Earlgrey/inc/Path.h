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
		static BOOL IsPathRooted(const xwstring& path);
		static BOOL IsPathRooted(const xstring& path);
		static xwstring Combine(const xwstring& path1, const xwstring& path2);
		static xstring Combine(const xstring& path1, const xstring& path2);
		static BOOL Exists(const xwstring& path);
		static BOOL Exists(const xstring& path);
		
		static xwstring GetDirectoryName(const xwstring& fullPath, BOOL strict);
		static xwstring GetDirectoryName(const xwstring& fullPath);

		static BOOL IsDirectorySeparator(TCHAR c);

		static std::wstring GetFileName(const std::wstring& path);
		static std::string GetFileName(const std::string& path);
#ifndef EARLGREY_BUILD_STL_ALLOCATOR
		static xwstring GetFileName(const xwstring& path);
		static xstring GetFileName(const xstring& path);
#endif
		
		static _txstring ResolveFilePath(const _txstring& fileName, BOOL throwNotFoundException = FALSE);

		
	public: // Fields

		static const WCHAR AltDirectorySeparatorCharW = L'/';
		static const WCHAR DirectorySeparatorCharW = L'\\';
		static const WCHAR VolumeSeparatorCharW = L':';

		static const CHAR AltDirectorySeparatorCharA = '/';
		static const CHAR DirectorySeparatorCharA = '\\';
		static const CHAR VolumeSeparatorCharA = ':';

#ifdef _UNICODE
		static const WCHAR AltDirectorySeparatorChar = AltDirectorySeparatorCharW;
		static const WCHAR DirectorySeparatorChar = DirectorySeparatorCharW;
		static const WCHAR VolumeSeparatorChar = VolumeSeparatorCharW;
#else
		static const WCHAR AltDirectorySeparatorChar = AltDirectorySeparatorCharA;
		static const WCHAR DirectorySeparatorChar= DirectorySeparatorCharA;
		static const WCHAR VolumeSeparatorChar = VolumeSeparatorCharA;
#endif

		template<typename T>
		struct SeparatorChar
		{

		};

		template<>
		struct SeparatorChar<WCHAR>
		{
			static const WCHAR Directory = DirectorySeparatorCharW;
			static const WCHAR AltDirectory = AltDirectorySeparatorCharW;
			static const WCHAR Volume = VolumeSeparatorCharW;
		};

		template<>
		struct SeparatorChar<CHAR>
		{
			static const CHAR Directory = DirectorySeparatorCharA;
			static const CHAR AltDirectory = AltDirectorySeparatorCharA;
			static const CHAR Volume = VolumeSeparatorCharA;
		};

	};
}