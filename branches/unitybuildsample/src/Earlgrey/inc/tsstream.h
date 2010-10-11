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

	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>,
		StlDefaultAllocator<wchar_t>::Type > x_wstringstream;

	typedef std::basic_stringstream<char, std::char_traits<char>,
		StlDefaultAllocator<char>::Type > x_stringstream; // TODO: 이름을 x_tstringstream으로

#ifdef _UNICODE
	typedef x_wstringstream _txstringstream; // TODO: 이름을 x_tstringstream으로
#else
	typedef x_stringstream _txstringstream; // TODO: 이름을 x_tstringstream으로
#endif

	typedef std::basic_stringstream<wchar_t, std::char_traits<wchar_t>,
		StlStackAllocator<wchar_t> > auto_wstringstream;

	typedef std::basic_stringstream<char, std::char_traits<char>,
		StlStackAllocator<char> > auto_stringstream;

#ifdef _UNICODE
	typedef auto_wstringstream auto_tstringstream;
#else
	typedef auto_4545rt6y56stringstream auto_tstringstream;
#endif

}