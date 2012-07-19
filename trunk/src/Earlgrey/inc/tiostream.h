#pragma once
#include "tios.h"
#include <iostream>

namespace Earlgrey
{
#ifdef _UNICODE
	typedef std::wistream _tistream;
	typedef std::wostream _tostream;
#else
	typedef	std::istream _tistream;
	typedef std::ostream _tostream;
#endif
	//! \ref http://www.gamedev.net/community/forums/topic.asp?topic_id=367500
	typedef std::basic_ostream<TCHAR, std::char_traits<TCHAR> > _tostream;
	typedef std::basic_istream<TCHAR, std::char_traits<TCHAR> > _tistream;
		
	// std::cerr
	extern _tostream &_tcout;
	extern _tistream &_tcin;
	extern _tostream &_tcerr;
	extern _tostream &_tclog;

}

