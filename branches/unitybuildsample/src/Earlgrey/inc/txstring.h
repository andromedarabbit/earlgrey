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
	std::basic_string<wchar_t, std::char_traits<wchar_t>, StlStackAllocator<wchar_t> > 
	auto_wstring
	;


typedef 
	std::basic_string<char, std::char_traits<char>, StlDefaultAllocator<char>::Type >
	xstring
	;

typedef 
	std::basic_string<char, std::char_traits<char>, StlStackAllocator<char> >
	auto_string
	;


#ifdef _UNICODE
	typedef xwstring _txstring; // TODO : �̸��� x_tstring ���� �ٲٱ�
	typedef auto_wstring auto_tstring;
#else
	typedef	xstring _txstring;	// TODO : �̸��� x_tstring ���� �ٲٱ�
	typedef	auto_string auto_txstring;
#endif

}