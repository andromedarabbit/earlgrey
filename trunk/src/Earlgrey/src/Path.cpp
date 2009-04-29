#include "stdafx.h"
#include "Path.h"

#include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <errno.h>

#include "EarlgreyAssert.h"

namespace Earlgrey
{
	//! \todo �۾� ��
	BOOL Path::Exists(const _txstring& path)
	{
		EARLGREY_ASSERT(path.length() > 0);

		if( _taccess( path.c_str(), 00 ) != 0 ) // Check existence only.
			return FALSE;
		
		//! \todo ���� ó��
		// errno_t errCode = errno;

		// _stati64 �Լ��� '\\'�� ������ ��θ� ����� �ν� �� �Ѵ�.
		_txstring pathNotTerminatedWithPathSeparator;
		if(path[path.length() - 1] == TEXT('\\'))
			pathNotTerminatedWithPathSeparator = path.substr(0, path.length() - 1);
		else
			pathNotTerminatedWithPathSeparator = path;


		struct _stati64 status;

		if ( _tstati64(pathNotTerminatedWithPathSeparator.c_str(), &status) != 0 ) 
		{
			//! \todo ���� ó��
			errno_t errCode = errno;
			if(errCode == ENOENT) // the filename or path could not be found
			{
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

	//! \todo strict == TRUE �� �� 
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
}
