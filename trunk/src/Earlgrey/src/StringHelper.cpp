#include "stdafx.h"
#include "StringHelper.h"

namespace Earlgrey
{

	const _txstring StringHelper::WhitespaceChars = TEXT("'\t\n\v\f\r ");
#if defined(_UNICODE)		
		// \todo 몇 개 더 있는데 자료를 찾아봐야 한다.
		// \note warning C4566: 유니버설 문자 이름 '\u2028'(으)로 표현되는 문자는 현재 코드 페이지(949)에서 표현할 수 없습니다.
		// , L'\x0085', L'\x00a0', L'\u2028', L'\u2029'
#endif		

	_txstring StringHelper::TrimHelper(const _txstring& str, const _txstring& trimChars, int trimType)
	{
		size_t num = str.length() - 1;
		size_t startIndex = 0;
		if (trimType != 1)
		{
			startIndex = 0;
			while (startIndex < str.length())
			{
				size_t index = 0;
				TCHAR ch = str[startIndex];
				index = 0;
				while (index < trimChars.length())
				{
					if (trimChars[index] == ch)
					{
						break;
					}
					index++;
				}
				if (index == trimChars.length())
				{
					break;
				}
				startIndex++;
			}
		}
		if (trimType != 0)
		{
			num = str.length() - 1;
			while (num >= startIndex)
			{
				size_t num4 = 0;
				TCHAR ch2 = str[num];
				num4 = 0;
				while (num4 < trimChars.length())
				{
					if (trimChars[num4] == ch2)
					{
						break;
					}
					num4++;
				}
				if (num4 == trimChars.length())
				{
					break;
				}
				num--;
			}
		}
		size_t length = (num - startIndex) + 1;
		if (length == str.length())
		{
			return str;
		}
		if (length == 0)
		{
			return TEXT("");
		}
		// return this.InternalSubString(startIndex, length, false);
		return str.substr(startIndex, length);
	}


	_txstring StringHelper::Trim(const _txstring& str)
	{
		return TrimHelper(str, WhitespaceChars, TrimBoth);
	}

	_txstring StringHelper::Trim(const _txstring& str, const _txstring& trimChars)
	{
		if (trimChars.length() == 0)
		{
			return TrimHelper(str, WhitespaceChars, TrimBoth);
		}
		return TrimHelper(str, trimChars, TrimBoth);
	}

	_txstring StringHelper::TrimStart(const _txstring& str)
	{
		return TrimHelper(str, WhitespaceChars, TrimHead);
	}

	_txstring StringHelper::TrimStart(const _txstring& str, const _txstring& trimChars)
	{
		if (trimChars.length() == 0)
		{
			return TrimStart(str);
		}
		return TrimHelper(str, trimChars, TrimHead);
	}

	_txstring StringHelper::TrimEnd(const _txstring& str)
	{
		return TrimHelper(str, WhitespaceChars, TrimTail);
	}

	_txstring StringHelper::TrimEnd(const _txstring& str, const _txstring& trimChars)
	{
		if (trimChars.length() == 0)
		{
			return TrimEnd(str);
		}
		return TrimHelper(str, trimChars, TrimTail);
	}


	BOOL StringHelper::StartsWith(const _txstring& str, const _txstring& prefix)
	{
		return StartsWith<STRCMP_ORDINAL>(str, prefix);
	}

	BOOL StringHelper::EndsWith(const _txstring& str, const _txstring& suffix)
	{
		return EndsWith<STRCMP_ORDINAL>(str, suffix);
	}

}