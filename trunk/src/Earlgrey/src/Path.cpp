#include "stdafx.h"
#include "Path.h"

#include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <errno.h>

#include "EarlgreyAssert.h"
#include "Environment.h"
#include "File.h"

namespace Earlgrey
{
	namespace
	{
		template<typename T>
		BOOL PathIsPathRooted(const T& path)
		{
			// CheckInvalidPathChars(path);
			size_t length = path.length();

			if (length >= 1 )
			{
				if(path[0] == Path::SeparatorChar<T::value_type>::Directory || path[0] == Path::SeparatorChar<T::value_type>::AltDirectory)
					return TRUE;
			}

			if (length >= 2 && path[1] == Path::SeparatorChar<T::value_type>::Volume)
			{
				return TRUE;
			}

			return FALSE;
		}
	}

	BOOL Path::IsPathRooted(const xstring& path)
	{
		return PathIsPathRooted(path);
	}

	BOOL Path::IsPathRooted(const xwstring& path)
	{
		return PathIsPathRooted(path);
	}

	

	BOOL Path::IsDirectorySeparator(const TCHAR c)
	{
		if (c != DirectorySeparatorChar)
		{
			return (c == AltDirectorySeparatorChar);
		}
		return TRUE;
	}

	namespace
	{
		template<typename T>
		T CombinePaths(const T& path1, const T& path2)
		{
			// CheckInvalidPathChars(path1);
			// CheckInvalidPathChars(path2);


			if(path2.length() == 0)
				return path1;

			if(path1.length() == 0)
				return path2;

			if (PathIsPathRooted(path2))
			{
				return path2;
			}

			T::value_type ch = path1[path1.length() - 1];
			if (((ch != Path::SeparatorChar<T::value_type>::Directory) && (ch != Path::SeparatorChar<T::value_type>::AltDirectory)) && (ch != Path::SeparatorChar<T::value_type>::Volume))
			{
				return (path1 + Path::SeparatorChar<T::value_type>::Directory + path2);
			}
			return (path1 + path2);		
		}
	}

	//! \todo 작업 중
	xstring Path::Combine(const xstring& path1, const xstring& path2)
	{
		return CombinePaths(path1, path2);
	}

	xwstring Path::Combine(const xwstring& path1, const xwstring& path2)
	{
		return CombinePaths(path1, path2);
	}



	//! \todo 작업 중
	BOOL Path::Exists(const xstring& path)
	{		 
		return Exists(String::ToUnicode(path));
	}

	BOOL Path::Exists(const xwstring& path)
	{
		EARLGREY_ASSERT(path.length() > 0);

		if( _waccess( path.c_str(), 00 ) != 0 ) // Check existence only.
			return FALSE;
		
		//! \todo 오류 처리
		// errno_t errCode = errno;

		// _stati64 함수는 '\\'로 끝나는 경로를 제대로 인식 못 한다.
		xwstring pathNotTerminatedWithPathSeparator;
		if(path[path.length() - 1] == L'\\')
			pathNotTerminatedWithPathSeparator = path.substr(0, path.length() - 1);
		else
			pathNotTerminatedWithPathSeparator = path;


		struct _stati64 status;

		if ( _wstati64(pathNotTerminatedWithPathSeparator.c_str(), &status) != 0 ) 
		{
			//! \todo 오류 처리
			errno_t errCode = errno;
			if(errCode == ENOENT) // the filename or path could not be found
			{
				return FALSE;
			}

			EARLGREY_ASSERT(errCode != EINVAL); // invalid parameters

			return FALSE;
		}

		if ( status.st_mode & S_IFDIR ) // directory
			return TRUE;

		if ( status.st_mode & _S_IFREG ) // File
			return TRUE;

		return FALSE;		
	}

	//! \todo strict == TRUE 일 때 
	xwstring Path::GetDirectoryName(const xwstring& fullPath, BOOL strict)
	{
		EARLGREY_ASSERT(fullPath.length() > 0);

		if(strict)
		{
			// TODO
			throw std::exception("Not yet implemented!");
		}

		// if(fullPath[fullPath.length() - 1] != TEXT('\\'))
		// return fullPath;

		std::size_t found = fullPath.rfind(L'\\');
		if (found != xwstring::npos)
		{
			return fullPath.substr(0, found);
		}
		return fullPath;
	}

	xwstring Path::GetDirectoryName(const xwstring& fullPath)
	{
		return GetDirectoryName(fullPath, FALSE);
	}

	namespace
	{
		template <typename T>
		T GetFileNameT(const T& path)
		{
			if (path.empty() == false)
			{
				// CheckInvalidPathChars(path);
				T::size_type length = path.length();
				T::size_type num2 = length;
				while (--num2 >= 0)
				{
					T::value_type ch = path[num2];
					if (((ch == Path::DirectorySeparatorChar) || (ch == Path::AltDirectorySeparatorChar)) || (ch == Path::VolumeSeparatorChar))
					{
						return path.substr(num2 + 1, (length - num2) - 1);
					}
				}
			}
			return path;
		}
	}

#ifndef EARLGREY_BUILD_STL_ALLOCATOR
	xwstring Path::GetFileName(const xwstring& path)
	{
		return GetFileNameT<xwstring>(path);
	}

	xstring Path::GetFileName(const xstring& path)
	{
		return GetFileNameT<xstring>(path);
	}
#endif

	std::wstring Path::GetFileName(const std::wstring& path)
	{
		return GetFileNameT<std::wstring>(path);
	}

	std::string Path::GetFileName(const std::string& path)
	{
		return GetFileNameT<std::string>(path);
	}

	namespace 
	{
		_txstring ResolveFilePathWithNoException(const _txstring& fileName)
		{
			if(Path::IsPathRooted(fileName))
			{
				return fileName;
			}

			_txstring currentDir(Environment::CurrentDirectory());
			_txstring filePath(Path::Combine(currentDir, fileName));
			if(File::Exists(filePath))
			{
				return filePath;
			}

			_txstring baseDir(Environment::BaseDirectory());
			return Path::Combine(baseDir, fileName);
		}
	}

	// TODO: 커스텀 예외로 고칠 것
	_txstring Path::ResolveFilePath(const _txstring& fileName, BOOL throwNotFoundException)
	{
		if(fileName.empty())
		{
			throw std::invalid_argument("fileName shouldn't be empty!");
		}

		_txstring filePath = ResolveFilePathWithNoException(fileName);
		if(throwNotFoundException == TRUE && File::Exists(filePath) == FALSE)
		{
			throw std::exception();
		}
		return filePath;
	}
}
