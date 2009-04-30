#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"

#ifdef min
#undef min // std::min 을 쓰려면 WinDef.h 에 정의된 매크로를 없애야 한다.
#endif

namespace Earlgrey
{
	enum E_StringComparison
	{
		STRCMP_CURRENT_CULTURE
		, STRCMP_CURRENT_CULTURE_IGNORECASE
		// , InvariantCulture
		// , InvariantCultureIgnoreCase
		, STRCMP_ORDINAL
		, STRCMP_ORDINAL_IGNORECASE
	};

	template<E_StringComparison flag>
	class StringComparison
	{
	public:
		enum { value = flag };

		int Compare(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const;
		int Compare(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const;

		BOOL Equals(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const;

		BOOL Equals(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const;
	};

	template<>
	class StringComparison<STRCMP_CURRENT_CULTURE>
	{
	public:
		enum { value = STRCMP_CURRENT_CULTURE };

		inline LCID Locale() const
		{
			return GetThreadLocale();
		}

		inline DWORD ComparisonFlags() const
		{
			return 0;
		}

		int Compare(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			int retValue = CompareString(Locale(), ComparisonFlags(), str1, cchCount1, str2, cchCount2);
			if(retValue == 0)
			{
				//! \todo 오류 처리
				// GetLastError - ERROR_INVALID_FLAGS, ERROR_INVALID_PARAMETER
			}
			return retValue;
		}

		int Compare(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			return Compare(str1, -1, str2, -1);
		}

		inline BOOL Equals(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			return Equals(str1, -1, str2, -1);
		}

		inline BOOL Equals(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}
	};

	template<>
	class StringComparison<STRCMP_CURRENT_CULTURE_IGNORECASE>
	{
	public:
		enum { value = STRCMP_CURRENT_CULTURE_IGNORECASE };

		inline LCID Locale() const
		{
			return GetThreadLocale();
		}

		inline DWORD ComparisonFlags() const
		{
			return NORM_IGNORECASE;
		}

		int Compare(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			int retValue = CompareString(Locale(), ComparisonFlags(), str1, cchCount1, str2, cchCount2);
			if(retValue == 0)
			{
				//! \todo 오류 처리
				// GetLastError - ERROR_INVALID_FLAGS, ERROR_INVALID_PARAMETER
			}
			return retValue;
		}

		int Compare(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			return Compare(str1, -1, str2, -1);
		}

		inline BOOL Equals(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			return Equals(str1, -1, str2, -1);
		}

		inline BOOL Equals(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}
	};

	template<>
	class StringComparison<STRCMP_ORDINAL>
	{
	public:
		enum { value = STRCMP_ORDINAL };

		int Compare(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			int cchCount = std::min(cchCount1, cchCount2);
			if(cchCount == 0)
				return TRUE;

			_txstring newStr1(str1, cchCount);
			_txstring newStr2(str2, cchCount);

			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmp를 쓴다.
			int retValue = lstrcmp(newStr1.c_str(), newStr2.c_str());
			return retValue + CSTR_EQUAL;
		}

		int Compare(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmp를 쓴다.
			int retValue = lstrcmp(str1, str2);
			return retValue + CSTR_EQUAL;
		}

		inline BOOL Equals(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			return Compare(str1, str2) == CSTR_EQUAL;
		}

		inline BOOL Equals(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}

	};

	template<>
	class StringComparison<STRCMP_ORDINAL_IGNORECASE>
	{
	public:
		enum { value = STRCMP_ORDINAL_IGNORECASE };

		int Compare(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			int cchCount = std::min(cchCount1, cchCount2);
			if(cchCount == 0)
				return TRUE;

			_txstring newStr1(str1, cchCount);
			_txstring newStr2(str2, cchCount);

			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmpi를 쓴다.
			int retValue = lstrcmpi(newStr1.c_str(), newStr2.c_str());
			return retValue + CSTR_EQUAL;
		}

		int Compare(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmpi를 쓴다.
			int retValue = lstrcmpi(str1, str2);
			return retValue + CSTR_EQUAL;
		}

		inline BOOL Equals(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			return Compare(str1, str2) == CSTR_EQUAL;
		}

		inline BOOL Equals(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}
	};
}