#pragma once
#include "StringComparison.hpp"
#include "numeric_cast.hpp"

namespace Earlgrey {
namespace String {
	
	//! \todo _locale_t는 c에서 쓰는 거라... LCID로 대체가능하면 좋겠다.	//! \todo in place 메서드가 많지 않다. 필요하면 더 구현하자.
	class StringHelper : private Uncopyable
	{
	private: // Fields
		enum E_TrimType
		{
			TrimHead = 0
			, TrimTail = 1
			, TrimBoth = 2
		};

	private:
		explicit StringHelper(); // 구현 안 함

		static _txstring TrimHelper(const _txstring& str, const _txstring& trimChars, E_TrimType trimType);

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

		static void ToLower(_txstring* strPtr);
		static void ToLower(_txstring* strPtr, _locale_t locale);
		static _txstring ToLower(_txstring str);
		static _txstring ToLower(_txstring str, _locale_t locale);

		static void ToUpper(_txstring* strPtr);
		static void ToUpper(_txstring* strPtr, _locale_t locale);
		static _txstring ToUpper(_txstring str);
		static _txstring ToUpper(_txstring str, _locale_t locale);

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


	// Alias
	inline _txstring Trim(const _txstring& str)
	{
		return StringHelper::Trim(str);
	}
	inline _txstring Trim(const _txstring& str, const _txstring& trimChars)
	{
		return StringHelper::Trim(str, trimChars);
	}

	inline _txstring TrimStart(const _txstring& str)
	{
		return StringHelper::TrimStart(str);
	}
	inline _txstring TrimStart(const _txstring& str, const _txstring& trimChars)
	{
		return StringHelper::TrimStart(str, trimChars);
	}

	inline _txstring TrimEnd(const _txstring& str)
	{
		return StringHelper::TrimEnd(str);
	}
	inline _txstring TrimEnd(const _txstring& str, const _txstring& trimChars)
	{
		return StringHelper::TrimEnd(str, trimChars);
	}

	inline void ToLower(_txstring* strPtr)
	{
		return StringHelper::ToLower(strPtr);
	}
	inline void ToLower(_txstring* strPtr, _locale_t locale)
	{
		return StringHelper::ToLower(strPtr, locale);
	}
	inline _txstring ToLower(_txstring str)
	{
		return StringHelper::ToLower(str);
	}
	inline _txstring ToLower(_txstring str, _locale_t locale)
	{
		return StringHelper::ToLower(str, locale);
	}


	inline void ToUpper(_txstring* strPtr)
	{
		return StringHelper::ToUpper(strPtr);
	}
	inline void ToUpper(_txstring* strPtr, _locale_t locale)
	{
		return StringHelper::ToUpper(strPtr, locale);
	}
	inline _txstring ToUpper(_txstring str)
	{
		return StringHelper::ToUpper(str);
	}
	inline _txstring ToUpper(_txstring str, _locale_t locale)
	{
		return StringHelper::ToUpper(str, locale);
	}

	template<E_StringComparison flag>
	inline BOOL StartsWith(const TCHAR * str, const size_t strLength, const TCHAR * prefix, const size_t prefixLength)
	{
		return StringHelper::StartsWith<flag>(str, strLength, prefix, prefixLength);
	}

	template<E_StringComparison flag>
	static BOOL StartsWith(const _txstring& str, const _txstring& prefix)
	{
		return StringHelper::StartsWith<flag>(str, prefix);
	}

	inline BOOL StartsWith(const _txstring& str, const _txstring& prefix)
	{
		return StringHelper::StartsWith(str, prefix);
	}



	template<E_StringComparison flag>
	inline BOOL EndsWith(const TCHAR * str, const size_t strLength, const TCHAR * suffix, const size_t suffixLength)
	{
		StringHelper::EndsWith<flag>(str, strLength, suffix, suffixLength);
	}

	template<E_StringComparison flag>
	inline BOOL EndsWith(const _txstring& str, const _txstring& suffix)
	{
		return StringHelper::EndsWith<flag>(str, suffix);
	}

	inline BOOL EndsWith(const _txstring& str, const _txstring& suffix)
	{
		return StringHelper::EndsWith(str, suffix);
	}

	// independent functions
	inline BOOL IsNullOrEmpty(const TCHAR * const str)
	{
		// TODO: _tcslen 에서 NULL을 아아서 확인하지 않을까?
		return str == NULL
			|| _tcslen(str) == 0
			;
	}


	// Encoding
	// MultiByteToWideChar
	const WCHAR * const ToUnicode(const CHAR * src, int srcBytes, UINT codePage = CP_THREAD_ACP);

	template<class _Ax>
	inline const WCHAR * const ToUnicode(
		const std::basic_string<char, std::char_traits<char>, _Ax>& src
		, UINT codePage = CP_THREAD_ACP
		)
	{
		const int bytes = EARLGREY_NUMERIC_CAST<int>(src.length() );
		return ToUnicode(src.c_str(), bytes, codePage);
	}

	// 이름과 달리 Unicode 문자열을 Unicode 문자열로 바꾸는(?) 메서드이다.
	// _UNICODE의 정의 여부에 따라 #if를 쓰지 않으려고 만들었다. 
	// _txstring의 경우를 생각하면 된다.
	template<class _Ax>
	inline const WCHAR * const ToUnicode(
		const std::basic_string<wchar_t, std::char_traits<wchar_t>, _Ax>& src
		, UINT codePage = CP_THREAD_ACP
		)
	{
		return src.c_str();
	}


	const CHAR * const FromUnicode(const WCHAR * src, int srcBytes, UINT codePage = CP_THREAD_ACP);

	template<class _Ax>
	inline const CHAR * const FromUnicode(
		const std::basic_string<wchar_t, std::char_traits<wchar_t>, _Ax>& src
		, UINT codePage = CP_THREAD_ACP
		)
	{
		const int bytes = EARLGREY_NUMERIC_CAST<int>(src.length() * sizeof(TCHAR));
		return FromUnicode(src.c_str(), bytes, codePage);
	}

	// 이름과 달리 Ansi 문자열을 Ansi 문자열로 바꾸는(?) 메서드이다.
	// _UNICODE의 정의 여부에 따라 #if를 쓰지 않으려고 만들었다. 
	// _txstring의 경우를 생각하면 된다.
	template<class _Ax>
	inline const CHAR * const FromUnicode(
		const std::basic_string<char, std::char_traits<char>, _Ax>& src
		, UINT /* codePage = CP_THREAD_ACP */
		)
	{
		return src.c_str();
	}
}
}