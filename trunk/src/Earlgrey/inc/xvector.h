#pragma once 
#include "StlCustomAllocator.hpp"

#include <vector>


namespace Earlgrey
{
	template<typename T>
	struct xvector
	{
		typedef 
			std::vector<T, typename StlDefaultAllocator< T >::Type> 
			Type; 
	};

}