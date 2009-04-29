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
			//! \todo 오류 처리
			errno_t errCode = errno;
			if(errCode == ENOENT) // the filename or path could not be found
			{
			}

			EARLGREY_ASSERT(errCode != EINVAL); // invalid parameters

			return FALSE;
		}

		if ( status.st_mode & _S_IFREG ) // File
			return TRUE;

		return FALSE;		
	}

}