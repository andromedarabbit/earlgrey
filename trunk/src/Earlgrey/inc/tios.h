#pragma once 
#include <ios>

namespace Earlgrey
{
	
#ifdef _UNICODE
	typedef std::wios _tios;
#else
	typedef	std::ios _tios;
#endif

}