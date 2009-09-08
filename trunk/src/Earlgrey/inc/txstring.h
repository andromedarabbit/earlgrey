#pragma once
#include "StlCustomAllocator.hpp"

#include <string>


namespace Earlgrey
{
typedef 
	std::basic_string<wchar_t, std::char_traits<wchar_t>, StlDefaultAllocator<wchar_t>::Type > 
	xwstring
	;

typedef 
	std::basic_string<char, std::char_traits<char>, StlDefaultAllocator<char>::Type >
	xstring
	;

#ifdef _UNICODE
	typedef xwstring _txstring;
#else
	typedef	xstring _txstring;
#endif

}