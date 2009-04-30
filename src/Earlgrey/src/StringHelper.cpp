#include "stdafx.h"
#include "StringHelper.h"


namespace Earlgrey
{
	BOOL StringHelper::StartsWith(const _txstring& str, const _txstring& prefix)
	{
		return StartsWith<STRCMP_ORDINAL>(str, prefix);
	}

	BOOL StringHelper::EndsWith(const _txstring& str, const _txstring& suffix)
	{
		return EndsWith<STRCMP_ORDINAL>(str, suffix);
	}

}
