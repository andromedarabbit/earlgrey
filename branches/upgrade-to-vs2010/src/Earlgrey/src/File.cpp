#include "stdafx.h"
#include "File.h"
#include "EarlgreyAssert.h"

// #include <io.h>   // For access().
#include <sys/types.h>  // For stat().
#include <sys/stat.h>   // For stat().
#include <errno.h>


namespace Earlgrey
{
	BOOL File::Exists(const _txstring& path)
	{
		EARLGREY_ASSERT(path.length() > 0);

		struct _stati64 status;

		if ( _tstati64(path.c_str(), &status) != 0 ) 
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


	BOOL File::Delete(const _txstring& path)
	{
		EARLGREY_ASSERT(path.length() > 0);

		if( ::DeleteFile(path.c_str()) == 0 ) // zero indicates failure
		{
			// TODO 坷幅 贸府
			// GetLastError
			return FALSE;
		}

		return TRUE;		
	}

	BOOL File::Copy(
		const _txstring& sourceFileName,
		const _txstring& destFileName,
		BOOL overwrite
		)
	{
		EARLGREY_ASSERT(sourceFileName.length() > 0);
		EARLGREY_ASSERT(destFileName.length() > 0);

		if( ::CopyFile(sourceFileName.c_str(), destFileName.c_str(), !overwrite) == 0 ) // zero indicates failure
		{
			// TODO 坷幅 贸府
			// GetLastError
			return FALSE;
		}

		return TRUE;	
	}
}