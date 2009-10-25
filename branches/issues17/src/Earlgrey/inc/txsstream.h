#pragma once

#include <sstream>
#include "StlCustomAllocator.hpp"

namespace Earlgrey
{
	// <sstream>
	typedef 
		std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, StlDefaultAllocator<wchar_t>::Type > 
		xwstringbuf
		;

	typedef 
		std::basic_stringbuf<char, std::char_traits<char>, StlDefaultAllocator<char>::Type > 
		xstringbuf
		;

#ifdef _UNICODE
	typedef xwstringbuf _txstringbuf;
#else
	typedef	xstringbuf _txstringbuf;
#endif

	typedef 
		std::basic_istringstream<wchar_t, std::char_traits<wchar_t>, StlDefaultAllocator<wchar_t>::Type > 
		xwistringstream
		;

	typedef 
		std::basic_istringstream<char, std::char_traits<char>, StlDefaultAllocator<char>::Type > 
		xistringstream
		;

#ifdef _UNICODE
	typedef xwistringstream _txistringstream;
#else
	typedef xistringstream _txistringstream;
#endif

	typedef 
		std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, StlDefaultAllocator<wchar_t>::Type > 
		xwostringstream;

	typedef 
		std::basic_ostringstream<char, std::char_traits<char>, StlDefaultAllocator<char>::Type > 
		xostringstream
		;

#ifdef _UNICODE
	typedef xwostringstream _txostringstream;
#else
	typedef	xostringstream _txostringstream;
#endif


	typedef 
		std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, StlDefaultAllocator<wchar_t>::Type > 
		xwstringstream
		;

	typedef 
		std::basic_stringstream<char, std::char_traits<char>, StlDefaultAllocator<char>::Type > 
		xstringstream
		;

#ifdef _UNICODE
	typedef xwstringstream _txstringstream;
#else
	typedef	xstringstream _txstringstream;
#endif
}