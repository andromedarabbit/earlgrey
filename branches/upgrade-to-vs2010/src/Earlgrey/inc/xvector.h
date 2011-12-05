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

	template<typename T>
	struct auto_vector
	{
		typedef 
			std::vector<T, StlStackAllocator< T > > 
			Type; 
	};


}