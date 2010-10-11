#pragma once

#include <sstream>
#include "StlCustomAllocator.hpp"
#include "StackAllocator.h"

namespace Earlgrey
{
	// <sstream>
	typedef 
		std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, StlDefaultAllocator<wchar_t>::Type > 
		xwstringbuf
		;

	typedef 
		std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, StlStackAllocator<wchar_t> > 
		auto_wstringbuf
		;

	typedef 
		std::basic_stringbuf<char, std::char_traits<char>, StlDefaultAllocator<char>::Type > 
		xstringbuf
		;
	
	typedef 
		std::basic_stringbuf<char, std::char_traits<char>, StlStackAllocator<char> > 
		auto_stringbuf
		;

#ifdef _UNICODE
	typedef xwstringbuf _txstringbuf; // TODO: 이름 바꾸기
	typedef auto_wstringbuf auto_tstringbuf;
#else
	typedef	xstringbuf _txstringbuf; // TODO: 이름 바꾸기
	typedef auto_stringbuf auto_tstringbuf;
#endif

	typedef 
		std::basic_istringstream<wchar_t, std::char_traits<wchar_t>, StlDefaultAllocator<wchar_t>::Type > 
		xwistringstream
		;

	typedef 
		std::basic_istringstream<wchar_t, std::char_traits<wchar_t>, StlStackAllocator<wchar_t> > 
		auto_wistringstream
		;

	typedef 
		std::basic_istringstream<char, std::char_traits<char>, StlDefaultAllocator<char>::Type > 
		xistringstream
		;

	typedef 
		std::basic_istringstream<char, std::char_traits<char>, StlStackAllocator<char> > 
		auto_istringstream
		;

#ifdef _UNICODE
	typedef xwistringstream _txistringstream;
	typedef auto_wistringstream auto_tistringstream;
#else
	typedef xistringstream _txistringstream;
	typedef auto_istringstream auto_tistringstream;
#endif

	typedef 
		std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, StlDefaultAllocator<wchar_t>::Type > 
		xwostringstream;

	typedef 
		std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, StlStackAllocator<wchar_t> > 
		auto_wostringstream;

	typedef 
		std::basic_ostringstream<char, std::char_traits<char>, StlDefaultAllocator<char>::Type > 
		xostringstream
		;

	typedef 
		std::basic_ostringstream<char, std::char_traits<char>, StlStackAllocator<char> > 
		auto_ostringstream
		;

#ifdef _UNICODE
	typedef xwostringstream _txostringstream; 
	typedef auto_wostringstream auto_tostringstream;
#else
	typedef	xostringstream _txostringstream;
	typedef auto_ostringstream auto_tostringstream;
#endif


	typedef 
		std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, StlDefaultAllocator<wchar_t>::Type > 
		xwstringstream
		;

	typedef 
		std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, StlStackAllocator<wchar_t> > 
		auto_wstringstream
		;

	typedef 
		std::basic_stringstream<char, std::char_traits<char>, StlDefaultAllocator<char>::Type > 
		xstringstream
		;

	typedef 
		std::basic_stringstream<char, std::char_traits<char>, StlStackAllocator<char> > 
		auto_stringstream
		;

#ifdef _UNICODE
	typedef xwstringstream _txstringstream;
	typedef auto_wstringstream auto_tstringstream;
#else
	typedef	xstringstream _txstringstream;
	typedef auto_stringstream auto_tstringstream;
#endif
}