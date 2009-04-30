#pragma once
#include "Uncopyable.h"
#include "StlCustom.h"

#ifdef min
#undef min // std::min �� ������ WinDef.h �� ���ǵ� ��ũ�θ� ���־� �Ѵ�.
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

		inline BOOL Equals(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			return Equals(str1, -1, str2, -1);
		}

		inline BOOL Equals(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			int retValue = CompareString(Locale(), ComparisonFlags(), str1, cchCount1, str2, cchCount2);
			if(retValue == 0)
			{
				//! \todo ���� ó��
				// GetLastError - ERROR_INVALID_FLAGS, ERROR_INVALID_PARAMETER
			}
			return retValue == CSTR_EQUAL;
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

		inline BOOL Equals(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			return Equals(str1, -1, str2, -1);
		}

		inline BOOL Equals(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			int retValue = CompareString(Locale(), ComparisonFlags(), str1, cchCount1, str2, cchCount2);
			if(retValue == 0)
			{
				//! \todo ���� ó��
				// GetLastError - ERROR_INVALID_FLAGS, ERROR_INVALID_PARAMETER
			}
			return retValue == CSTR_EQUAL;
		}
	};

	template<>
	class StringComparison<STRCMP_ORDINAL>
	{
	public:
		enum { value = STRCMP_ORDINAL };

		inline BOOL Equals(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			// CompareStringOrdinal �Լ��� ��Ÿ���� �����ϹǷ� lstrcmp�� ����.
			return lstrcmp(str1, str2) == 0;
		}

		inline BOOL Equals(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			int cchCount = std::min(cchCount1, cchCount2);
			if(cchCount == 0)
				return TRUE;

			_txstring newStr1(str1, cchCount);
			_txstring newStr2(str2, cchCount);
			
			return lstrcmp(newStr1.c_str(), newStr2.c_str()) == 0;
		}

	};

	template<>
	class StringComparison<STRCMP_ORDINAL_IGNORECASE>
	{
	public:
		enum { value = STRCMP_ORDINAL_IGNORECASE };

		inline BOOL Equals(__nullterminated const TCHAR * str1, __nullterminated const TCHAR * str2) const
		{
			return lstrcmpi(str1, str2) == 0;
		}

		inline BOOL Equals(const TCHAR * str1, int cchCount1, const TCHAR * str2, int cchCount2) const
		{
			int cchCount = std::min(cchCount1, cchCount2);
			if(cchCount == 0)
				return TRUE;

			_txstring newStr1(str1, cchCount);
			_txstring newStr2(str2, cchCount);

			return lstrcmpi(newStr1.c_str(), newStr2.c_str()) == 0;
		}
	};
}