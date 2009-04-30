#pragma once
#include "StlCustom.h"
#include "StringComparison.hpp"

namespace Earlgrey
{
	class StringHelper : private Uncopyable
	{
	private: // Fields
		static const int TrimBoth = 2;
		static const int TrimHead = 0;
		static const int TrimTail = 1;

	private:
		explicit StringHelper(); // 구현 안 함

		static _txstring TrimHelper(const _txstring& str, const _txstring& trimChars, int trimType);

	public: // Fields
		// static const TCHAR WhitespaceChars[];
		static const _txstring WhitespaceChars;
	
	public: // Methods
		static _txstring Trim(const _txstring& str);
		static _txstring Trim(const _txstring& str, const _txstring& trimChars);

		static _txstring TrimStart(const _txstring& str);
		static _txstring TrimStart(const _txstring& str, const _txstring& trimChars);

		static _txstring TrimEnd(const _txstring& str);
		static _txstring TrimEnd(const _txstring& str, const _txstring& trimChars);






		template<E_StringComparison flag>
		static BOOL StartsWith(const TCHAR * str, const size_t strLength, const TCHAR * prefix, const size_t prefixLength)
		{
			StringComparison<flag> comparison;
			if(prefix == 0)
				return TRUE;

			if(strLength < prefixLength)
				return FALSE;

			return comparison.Equals(
				str
				, static_cast<int>(strLength)
				, prefix
				, static_cast<int>(prefixLength)
				);
		}

		template<E_StringComparison flag>
		static BOOL StartsWith(const _txstring& str, const _txstring& prefix)
		{
			return StartsWith<flag>(
				str.c_str()
				, str.length()
				, prefix.c_str()
				, prefix.length()
				);
		}

		static BOOL StartsWith(const _txstring& str, const _txstring& prefix);



		template<E_StringComparison flag>
		static BOOL EndsWith(const TCHAR * str, const size_t strLength, const TCHAR * suffix, const size_t suffixLength)
		{
			StringComparison<flag> comparison;
			if(suffixLength == 0)
				return TRUE;

			if(strLength < suffixLength)
				return FALSE;

			const TCHAR * strSuffix = str + (strLength - suffixLength);
			return comparison.Equals(strSuffix, suffix);
		}

		template<E_StringComparison flag>
		static BOOL EndsWith(const _txstring& str, const _txstring& suffix)
		{
			return EndsWith<flag>(
				str.c_str()
				, str.length()
				, suffix.c_str()
				, suffix.length()
				);
		}

		static BOOL EndsWith(const _txstring& str, const _txstring& suffix);



	};
}