#pragma once 
#include "StlCustomAllocator.hpp"

#include <list>

namespace Earlgrey
{

	template<typename T, typename Allocator = StlDefaultAllocator< T >::Type >
	struct xlist
	{
		typedef 
			std::list<T, Allocator> 
			Type; 
	};

}