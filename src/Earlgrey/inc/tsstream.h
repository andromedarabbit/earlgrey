#pragma once
#include "StlCustomAllocator.hpp"
#include <sstream>

namespace Earlgrey
{
#ifdef _UNICODE
	typedef std::wstringstream _tstringstream;
#else
	typedef	std::stringstream _tstringstream;
#endif

#ifdef _UNICODE
	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>,
		StlDefaultAllocator<wchar_t>::Type > _txstringstream;
#else
	typedef std::basic_stringstream<char, std::char_traits<char>,
		StlDefaultAllocator<char>::Type > _txstringstream;
#endif

}