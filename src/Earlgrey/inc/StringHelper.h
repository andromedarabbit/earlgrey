#pragma once
#include "Uncopyable.h"

namespace Earlgrey
{
	enum E_StringComparion
	{
		STRCMP_CURRENT_CULTURE
		, STRCMP_CURRENT_CULTURE_IGNORECASE
		// , InvariantCulture
		// , InvariantCultureIgnoreCase
		, STRCMP_ORDINAL
		, STRCMP_ORDINAL_IGNORECASE
	};

	template<E_StringComparion flag>
	struct StringComparion
	{
		enum { value = flag };

		DWORD ComparisonFlags() const;
	};

	template<>
	struct StringComparion<STRCMP_CURRENT_CULTURE>
	{
		enum { value = STRCMP_CURRENT_CULTURE };

		DWORD ComparisonFlags() const
		{
			return 0;
		}


	};

	class StringHelper : private Uncopyable
	{
	private:
		explicit StringHelper(); // 구현 안 함

	public:
			/*
		static BOOL StartsWith(const _txstring& obj, const TCHAR * const value, StringComparison comparison)
		{
			DWORD cmpFlags;

			// switch(comparison == 


			return FALSE;
		}

		static BOOL StartsWith(const _txstring& obj, const TCHAR * const value)
		{
			return StartsWith(obj, value, CurrentCulture);
		}
*/
	};
}