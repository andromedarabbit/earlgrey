#include "stdafx.h"
#include "StringHelper.h"

#pragma warning( push )
#pragma warning( disable : 4996 ) // memcpy_s가 아닌 memcpy를 쓴 부분 때문에...
#include <locale>
#pragma warning( pop )

#include <algorithm>

namespace Earlgrey {
namespace String {

	const _txstring StringHelper::WhitespaceChars = TEXT("\t\n\v\f\r ");
#if defined(_UNICODE)		
		// \todo 몇 개 더 있는데 자료를 찾아봐야 한다.
		// \note warning C4566: 유니버설 문자 이름 '\u2028'(으)로 표현되는 문자는 현재 코드 페이지(949)에서 표현할 수 없습니다.
		// , L'\x0085', L'\x00a0', L'\u2028', L'\u2029'
#endif		

	_txstring StringHelper::TrimHelper(const _txstring& str, const _txstring& trimChars, E_TrimType trimType)
	{
		if(str.length() == 0)
			return str;

		size_t num = str.length() - 1;
		size_t startIndex = 0;
		if (trimType != TrimTail)
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
		if (trimType != TrimHead)
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

	namespace 
	{
		struct ToLowerWIthLocale
		{
		public:
			explicit ToLowerWIthLocale(_locale_t locale)
				: m_Locale(locale)
			{
			}

			int operator()(TCHAR ch)
			{
				return _totlower_l(ch, m_Locale);
			}
		private:
			_locale_t m_Locale;
		};

		struct ToUpperWIthLocale
		{
		public:
			explicit ToUpperWIthLocale(_locale_t locale)
				: m_Locale(locale)
			{
			}

			int operator()(TCHAR ch)
			{
				return _totupper_l(ch, m_Locale);
			}
		private:
			_locale_t m_Locale;
		};
	}

	void StringHelper::ToLower(_txstring* strPtr, _locale_t locale)
	{
		ToLowerWIthLocale charTrans(locale);

		_txstring& str = *strPtr;

		std::transform(
			str.begin()
			, str.end()
			, str.begin()
			, charTrans
			); 
	}

	void StringHelper::ToLower(_txstring* strPtr)
	{		
		_txstring& str = *strPtr;

		std::transform(
			str.begin()
			, str.end()
			, str.begin()
			, _totlower
			); 
	}

	_txstring StringHelper::ToLower(_txstring str, _locale_t locale)
	{
		ToLower(&str, locale);
		return str;
	}

	_txstring StringHelper::ToLower(_txstring str)
	{
		ToLower(&str);		
		return str;
	}

	void StringHelper::ToUpper(_txstring* strPtr, _locale_t locale)
	{
		ToUpperWIthLocale charTrans(locale);

		_txstring& str = *strPtr;

		std::transform(
			str.begin()
			, str.end()
			, str.begin()
			, charTrans
			); 
	}

	void StringHelper::ToUpper(_txstring* strPtr)
	{
		_txstring& str = *strPtr;

		std::transform(
			str.begin()
			, str.end()
			, str.begin()
			, _totupper
			); 
	}

	_txstring StringHelper::ToUpper(_txstring str, _locale_t locale)
	{
		ToUpper(&str, locale);
		return str;
	}

	_txstring StringHelper::ToUpper(_txstring str)
	{
		ToUpper(&str);		
		return str;
	}

	BOOL StringHelper::StartsWith(const xwstring& str, const xwstring& prefix)
	{
		return StartsWith<STRCMP_ORDINAL>(str, prefix);
	}

	BOOL StringHelper::StartsWith(const xstring& str, const xstring& prefix)
	{
		return StartsWith<STRCMP_ORDINAL>(str, prefix);
	}

	BOOL StringHelper::EndsWith(const xwstring& str, const xwstring& suffix)
	{
		return EndsWith<STRCMP_ORDINAL>(str, suffix);
	}

	BOOL StringHelper::EndsWith(const xstring& str, const xstring& suffix)
	{
		return EndsWith<STRCMP_ORDINAL>(str, suffix);
	}



	// Encoding
	const WCHAR * const ToUnicode(const CHAR * src, int srcBytes, UINT codePage)
	{
		static const int BUFFER_SIZE = MAX_PATH;
		static __declspec(thread) WCHAR BUFFER[BUFFER_SIZE]; // TODO: 임시

		if(srcBytes == 0)
		{
			BUFFER[0] = NULL;
			return BUFFER;
		}

		const int dstLength = ::MultiByteToWideChar(codePage, MB_PRECOMPOSED | MB_ERR_INVALID_CHARS, src, srcBytes, BUFFER, BUFFER_SIZE);
		if(dstLength == 0)
		{
			const DWORD errCode = GetLastError();
			DBG_UNREFERENCED_PARAMETER(errCode);
			/*
			ERROR_INSUFFICIENT_BUFFER 
			ERROR_INVALID_FLAGS 
			ERROR_INVALID_PARAMETER 
			ERROR_NO_UNICODE_TRANSLATION 
			*/
			BUFFER[0] = NULL;
			return BUFFER;
		}
		BUFFER[dstLength] = NULL;
		return BUFFER;
	}

	const CHAR * const FromUnicode(const WCHAR * src, int srcBytes, UINT codePage)
	{
		static const int BUFFER_SIZE = MAX_PATH;
		static __declspec(thread) CHAR BUFFER[BUFFER_SIZE]; // TODO: 임시

		if(srcBytes == 0)
		{
			BUFFER[0] = NULL;
			return BUFFER;
		}

		// TODO: 옵션 연구를 더 해봐야 할 듯
		BOOL usedDeafultChar = FALSE;
		const int dstLength = ::WideCharToMultiByte(codePage, 0, src, srcBytes, BUFFER, BUFFER_SIZE, NULL, &usedDeafultChar); 
		if(dstLength == 0)
		{
			const DWORD errCode = GetLastError();
			DBG_UNREFERENCED_PARAMETER(errCode);
			/*
			ERROR_INSUFFICIENT_BUFFER 
			ERROR_INVALID_FLAGS 
			ERROR_INVALID_PARAMETER 
			*/
			BUFFER[0] = NULL;
			return BUFFER;
		}

		// TODO
		if(usedDeafultChar)
		{

		}

		return BUFFER;
	}

// 	const CHAR * FromUnicode(const _txstring& src, UINT codePage)
// 	{
// #ifdef _UNICODE
// 		const int bytes = EARLGREY_NUMERIC_CAST<int>(src.length() * sizeof(TCHAR));
// 		return FromUnicode(src.c_str(), bytes, codePage);
// #else
// 		return src.c_str();
// #endif
// 	}
}
}