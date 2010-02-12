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
		StlDefaultAllocator<wchar_t>::Type > _txstringstream; // TODO: 이름을 x_tstringstream으로
#else
	typedef std::basic_stringstream<char, std::char_traits<char>,
		StlDefaultAllocator<char>::Type > _txstringstream; // TODO: 이름을 x_tstringstream으로
#endif

#ifdef _UNICODE
	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>,
		StlStackAllocator<wchar_t> > auto_tstringstream;
#else
	typedef std::basic_stringstream<char, std::char_traits<char>,
		StlStackAllocator<char> > auto_tstringstream;
#endif

}