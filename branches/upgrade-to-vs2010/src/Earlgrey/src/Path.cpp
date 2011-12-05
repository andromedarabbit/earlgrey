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
	BOOL Path::IsPathRooted(const _txstring& path)
	{
		// CheckInvalidPathChars(path);
		size_t length = path.length();

		if (length >= 1 )
		{
			if(path[0] == DirectorySeparatorChar || path[0] == AltDirectorySeparatorChar)
				return TRUE;
		}

		if (length >= 2 && path[1] == VolumeSeparatorChar)
		{
			return TRUE;
		}

		return FALSE;
	}


	BOOL Path::IsDirectorySeparator(const TCHAR c)
	{
		if (c != DirectorySeparatorChar)
		{
			return (c == AltDirectorySeparatorChar);
		}
		return TRUE;
	}

	//! \todo 작업 중
	 _txstring Path::Combine(const _txstring& path1, const _txstring& path2)
	 {
		 // CheckInvalidPathChars(path1);
		 // CheckInvalidPathChars(path2);


		if(path2.length() == 0)
			return path1;

		if(path1.length() == 0)
			return path2;

		if (IsPathRooted(path2))
		{
			return path2;
		}

		TCHAR ch = path1[path1.length() - 1];
		if (((ch != DirectorySeparatorChar) && (ch != AltDirectorySeparatorChar)) && (ch != VolumeSeparatorChar))
		{
			return (path1 + DirectorySeparatorChar + path2);
		}
		return (path1 + path2);
	 }

	//! \todo 작업 중
	BOOL Path::Exists(const _txstring& path)
	{
		EARLGREY_ASSERT(path.length() > 0);

		if( _taccess( path.c_str(), 00 ) != 0 ) // Check existence only.
			return FALSE;
		
		//! \todo 오류 처리
		// errno_t errCode = errno;

		// _stati64 함수는 '\\'로 끝나는 경로를 제대로 인식 못 한다.
		_txstring pathNotTerminatedWithPathSeparator;
		if(path[path.length() - 1] == TEXT('\\'))
			pathNotTerminatedWithPathSeparator = path.substr(0, path.length() - 1);
		else
			pathNotTerminatedWithPathSeparator = path;


		struct _stati64 status;

		if ( _tstati64(pathNotTerminatedWithPathSeparator.c_str(), &status) != 0 ) 
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
	_txstring Path::GetDirectoryName(const _txstring& fullPath, BOOL strict)
	{
		EARLGREY_ASSERT(fullPath.length() > 0);

		if(strict)
		{
		}

		// if(fullPath[fullPath.length() - 1] != TEXT('\\'))
		// return fullPath;

		std::size_t found = fullPath.rfind(TEXT('\\'));
		if (found != _txstring::npos)
		{
			return fullPath.substr(0, found);
		}
		return fullPath;
	}

	_txstring Path::GetDirectoryName(const _txstring& fullPath)
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
					TCHAR ch = path[num2];
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
	_txstring Path::GetFileName(const _txstring& path)
	{
		return GetFileNameT<_txstring>(path);
	}
#endif

	_tstring Path::GetFileName(const _tstring& path)
	{
		return GetFileNameT<_tstring>(path);
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
