#include "stdafx.h"
#include "EarlgreyGTest.h"
#include "StringHelper.h"

namespace Earlgrey
{
	void ASSERT_STARTS_WITH(const _txstring& expected, const _txstring& value)
	{
		using namespace Earlgrey::String;

		//const size_t valueLength = value.length();
		//const size_t prefixLength = expected.length();			

		//ASSERT_GE(valueLength, prefixLength);

		//// _txstring prefix = value.substr(value.length() - prefixLength, prefixLength);
		//_txstring prefix = value.substr(0, prefixLength);

		//BOOL startsWith = StartsWith<STRCMP_CURRENT_CULTURE_IGNORECASE>(prefix, expected);
		//ASSERT_TRUE2(endsWith);
		BOOL startsWith = StartsWith<STRCMP_ORDINAL>(value, expected);
		ASSERT_TRUE2(startsWith);
	}

	void ASSERT_ENDS_WITH(const _txstring& expected, const _txstring& value)
	{
		using namespace Earlgrey::String;

		const size_t valueLength = value.length();
		const size_t suffixLength = expected.length();			

		ASSERT_GE(valueLength, suffixLength);

		_txstring suffix = value.substr(value.length() - suffixLength, suffixLength);

		BOOL endsWith = EndsWith<STRCMP_ORDINAL>(suffix, expected);
		ASSERT_TRUE2(endsWith);
	}

	void ASSERT_CONTAINS(const _txstring& longStr, const _txstring& shortStr)
	{
		const size_t longLength = longStr.length();
		const size_t shortLength = shortStr.length();			

		ASSERT_GE(longLength, shortLength);

		size_t found = longStr.find(shortStr);
		ASSERT_NE(_txstring::npos, found);
	}

}