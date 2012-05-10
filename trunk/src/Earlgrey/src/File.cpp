#include "stdafx.h"
#include "File.h"
#include "EarlgreyAssert.h"

// #include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <errno.h>


namespace Earlgrey
{
	BOOL File::Exists(const xstring& path)
	{
		return Exists(String::ToUnicode(path));
	}

	BOOL File::Exists(const xwstring& path)
	{
		EARLGREY_ASSERT(path.length() > 0);

		struct _stati64 status;

		if ( _wstati64(path.c_str(), &status) != 0 ) 
		{
			//! \todo 坷幅 贸府
			errno_t errCode = errno;
			if(errCode == ENOENT) // the filename or path could not be found
			{
				return FALSE;
			}

			EARLGREY_ASSERT(errCode != EINVAL); // invalid parameters

			return FALSE;
		}

		if ( status.st_mode & _S_IFREG ) // File
			return TRUE;

		return FALSE;		
	}


	BOOL File::Delete(const xstring& path)
	{
		return Delete(String::ToUnicode(path));
	}

	BOOL File::Delete(const xwstring& path)
	{
		EARLGREY_ASSERT(path.length() > 0);

		if( ::DeleteFileW(path.c_str()) == 0 ) // zero indicates failure
		{
			// TODO 坷幅 贸府
			// GetLastError
			return FALSE;
		}

		return TRUE;		
	}

	BOOL File::Copy(
		const xstring& sourceFileName,
		const xstring& destFileName,
		BOOL overwrite
		)
	{
		return Copy(String::ToUnicode(sourceFileName), String::ToUnicode(destFileName), overwrite);
	}

	BOOL File::Copy(
		const xwstring& sourceFileName,
		const xwstring& destFileName,
		BOOL overwrite
		)
	{
		EARLGREY_ASSERT(sourceFileName.length() > 0);
		EARLGREY_ASSERT(destFileName.length() > 0);

		if( ::CopyFileW(sourceFileName.c_str(), destFileName.c_str(), !overwrite) == 0 ) // zero indicates failure
		{
			// TODO 坷幅 贸府
			// GetLastError
			return FALSE;
		}

		return TRUE;	
	}
}