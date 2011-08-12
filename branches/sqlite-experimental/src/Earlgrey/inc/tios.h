#pragma once 
#if (_MSC_VER >= 1600)
#	pragma warning(push)
#	pragma warning(disable: 4996)
#	include <ios>
#	pragma warning(pop)
#else
#	include <ios>
#endif

namespace Earlgrey
{
	
#ifdef _UNICODE
	typedef std::wios _tios;
#else
	typedef	std::ios _tios;
#endif

}