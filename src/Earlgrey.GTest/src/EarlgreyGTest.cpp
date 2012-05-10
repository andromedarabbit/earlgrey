#include "stdafx.h"
#include "EarlgreyGTest.h"
#include "StringHelper.h"

namespace Earlgrey
{
	namespace
	{
		template<typename T>
		inline void AssertStartsWith(const T& expected, const T& value)
		{
			using namespace Earlgrey::String;

			BOOL startsWith = StartsWith<STRCMP_ORDINAL>(value, expected);
			ASSERT_TRUE2(startsWith);
		}
	}
	
	void ASSERT_STARTS_WITH(const xwstring& expected, const xwstring& value)
	{
		AssertStartsWith(expected, value);
	}

	void ASSERT_STARTS_WITH(const xstring& expected, const xstring& value)
	{
		AssertStartsWith(expected, value);
	}


	namespace
	{
		template<typename T>
		inline void AssertEndsWith(const T& expected, const T& value)
		{
			using namespace Earlgrey::String;

			const size_t valueLength = value.length();
			const size_t suffixLength = expected.length();			

			ASSERT_GE(valueLength, suffixLength);

			T suffix = value.substr(value.length() - suffixLength, suffixLength);

			BOOL endsWith = EndsWith<STRCMP_ORDINAL>(suffix, expected);
			ASSERT_TRUE2(endsWith);
		}
	}

	void ASSERT_ENDS_WITH(const xwstring& expected, const xwstring& value)
	{
		AssertEndsWith(expected, value);
	}

	void ASSERT_ENDS_WITH(const xstring& expected, const xstring& value)
	{
		AssertEndsWith(expected, value);
	}

	namespace
	{
		template<typename T>
		inline void AssertContains(const T& longStr, const T& shortStr)
		{
			const size_t longLength = longStr.length();
			const size_t shortLength = shortStr.length();			

			ASSERT_GE(longLength, shortLength);

			size_t found = longStr.find(shortStr);
			ASSERT_NE(T::npos, found);
		}
	}

	void ASSERT_CONTAINS(const xwstring& longStr, const xwstring& shortStr)
	{
		AssertContains(longStr, shortStr);
	}

	void ASSERT_CONTAINS(const xstring& longStr, const xstring& shortStr)
	{
		AssertContains(longStr, shortStr);
	}

}