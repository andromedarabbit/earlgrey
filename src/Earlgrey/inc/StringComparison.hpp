#pragma once
#include "Uncopyable.h"
#include "txstring.h"
#include "Macros.h"
#include "Log.h"


// TODO: 중복코드 제거
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

		int Compare(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const;
		int Compare(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const;
		int Compare(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const;
		int Compare(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const;

		BOOL Equals(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const;
		BOOL Equals(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const;

		BOOL Equals(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const;
		BOOL Equals(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const;
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

		int Compare(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const
		{
			int retValue = CompareStringW(Locale(), ComparisonFlags(), str1, cchCount1, str2, cchCount2);
			if(retValue == 0)
			{
				// ERROR_INVALID_FLAGS, ERROR_INVALID_PARAMETER
				const DWORD errCode = GetLastError();
				const char * const errMsg = Log::ErrorMessageA(errCode);
				throw std::exception(errMsg);
			}
			return retValue;
		}

		// TODO: 중복 코드 
		int Compare(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const
		{
			int retValue = CompareStringA(Locale(), ComparisonFlags(), str1, cchCount1, str2, cchCount2);
			if(retValue == 0)
			{
				// ERROR_INVALID_FLAGS, ERROR_INVALID_PARAMETER
				const DWORD errCode = GetLastError();
				const char * const errMsg = Log::ErrorMessageA(errCode);
				throw std::exception(errMsg);
			}
			return retValue;
		}

		inline int Compare(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const
		{
			return Compare(str1, -1, str2, -1);
		}

		inline int Compare(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const
		{
			return Compare(str1, -1, str2, -1);
		}

		inline BOOL Equals(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const
		{
			return Equals(str1, -1, str2, -1);
		}

		inline BOOL Equals(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const
		{
			return Equals(str1, -1, str2, -1);
		}

		inline BOOL Equals(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}

		inline BOOL Equals(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const
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

		int Compare(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const
		{
			int retValue = CompareStringW(Locale(), ComparisonFlags(), str1, cchCount1, str2, cchCount2);
			if(retValue == 0)
			{
				// ERROR_INVALID_FLAGS, ERROR_INVALID_PARAMETER
				const DWORD errCode = GetLastError();
				const char * const errMsg = Log::ErrorMessageA(errCode);
				throw std::exception(errMsg);
			}
			return retValue;
		}

		int Compare(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const
		{
			int retValue = CompareStringA(Locale(), ComparisonFlags(), str1, cchCount1, str2, cchCount2);
			if(retValue == 0)
			{
				// ERROR_INVALID_FLAGS, ERROR_INVALID_PARAMETER
				const DWORD errCode = GetLastError();
				const char * const errMsg = Log::ErrorMessageA(errCode);
				throw std::exception(errMsg);
			}
			return retValue;
		}

		inline int Compare(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const
		{
			return Compare(str1, -1, str2, -1);
		}

		inline int Compare(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const
		{
			return Compare(str1, -1, str2, -1);
		}

		inline BOOL Equals(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const
		{
			return Equals(str1, -1, str2, -1);
		}

		inline BOOL Equals(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const
		{
			return Equals(str1, -1, str2, -1);
		}

		inline BOOL Equals(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}

		inline BOOL Equals(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}
	};

	template<>
	class StringComparison<STRCMP_ORDINAL>
	{
	public:
		enum { value = STRCMP_ORDINAL };

		int Compare(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const
		{
			int cchCount = std::min EARLGREY_PREVENT_MACRO_SUBSTITUTION (cchCount1, cchCount2);
			if(cchCount == 0)
				return TRUE;

			xwstring newStr1(str1, cchCount);
			xwstring newStr2(str2, cchCount);

			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmp를 쓴다.
			int retValue = lstrcmpW(newStr1.c_str(), newStr2.c_str());
			return retValue + CSTR_EQUAL;
		}

		int Compare(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const
		{
			int cchCount = std::min EARLGREY_PREVENT_MACRO_SUBSTITUTION (cchCount1, cchCount2);
			if(cchCount == 0)
				return TRUE;

			xstring newStr1(str1, cchCount);
			xstring newStr2(str2, cchCount);

			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmp를 쓴다.
			int retValue = lstrcmpA(newStr1.c_str(), newStr2.c_str());
			return retValue + CSTR_EQUAL;
		}


		int Compare(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const
		{
			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmp를 쓴다.
			int retValue = lstrcmpW(str1, str2);
			return retValue + CSTR_EQUAL;
		}

		int Compare(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const
		{
			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmp를 쓴다.
			int retValue = lstrcmpA(str1, str2);
			return retValue + CSTR_EQUAL;
		}

		inline BOOL Equals(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const
		{
			return Compare(str1, str2) == CSTR_EQUAL;
		}

		inline BOOL Equals(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const
		{
			return Compare(str1, str2) == CSTR_EQUAL;
		}


		inline BOOL Equals(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}

		inline BOOL Equals(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}

	};

	template<>
	class StringComparison<STRCMP_ORDINAL_IGNORECASE>
	{
	public:
		enum { value = STRCMP_ORDINAL_IGNORECASE };

		int Compare(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const
		{
			int cchCount = std::min EARLGREY_PREVENT_MACRO_SUBSTITUTION (cchCount1, cchCount2);
			if(cchCount == 0)
				return TRUE;

			xwstring newStr1(str1, cchCount);
			xwstring newStr2(str2, cchCount);

			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmpi를 쓴다.
			int retValue = lstrcmpiW(newStr1.c_str(), newStr2.c_str());
			return retValue + CSTR_EQUAL;
		}

		int Compare(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const
		{
			int cchCount = std::min EARLGREY_PREVENT_MACRO_SUBSTITUTION (cchCount1, cchCount2);
			if(cchCount == 0)
				return TRUE;

			xstring newStr1(str1, cchCount);
			xstring newStr2(str2, cchCount);

			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmpi를 쓴다.
			int retValue = lstrcmpiA(newStr1.c_str(), newStr2.c_str());
			return retValue + CSTR_EQUAL;
		}

		int Compare(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const
		{
			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmpi를 쓴다.
			int retValue = lstrcmpiW(str1, str2);
			return retValue + CSTR_EQUAL;
		}
		int Compare(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const
		{
			// CompareStringOrdinal 함수는 비스타부터 지원하므로 lstrcmpi를 쓴다.
			int retValue = lstrcmpiA(str1, str2);
			return retValue + CSTR_EQUAL;
		}

		inline BOOL Equals(__nullterminated const WCHAR * str1, __nullterminated const WCHAR * str2) const
		{
			return Compare(str1, str2) == CSTR_EQUAL;
		}

		inline BOOL Equals(__nullterminated const CHAR * str1, __nullterminated const CHAR * str2) const
		{
			return Compare(str1, str2) == CSTR_EQUAL;
		}

		inline BOOL Equals(const WCHAR * str1, int cchCount1, const WCHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}

		inline BOOL Equals(const CHAR * str1, int cchCount1, const CHAR * str2, int cchCount2) const
		{
			return Compare(str1, cchCount1, str2, cchCount2) == CSTR_EQUAL;
		}
	};
}