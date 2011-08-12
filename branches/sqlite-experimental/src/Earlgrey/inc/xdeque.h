#pragma once
#include "StlCustomAllocator.hpp"

#include <deque>


namespace Earlgrey
{
	template<typename T>
	struct xdeque
	{
		typedef 
			std::deque<T, typename StlDefaultAllocator< T >::Type> 
			Type; 
	};


	template<typename T>
	struct auto_deque
	{
		typedef 
			std::deque<T, typename StlStackAllocator< T > > 
			Type; 
	};
}