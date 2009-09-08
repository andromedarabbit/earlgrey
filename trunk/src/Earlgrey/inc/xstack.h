#pragma once 
#include "StlCustomAllocator.hpp"

#include <stack>


namespace Earlgrey
{
	template<typename T, typename Container = xdeque<T>::Type >
	struct xstack
	{
		typedef 
			std::stack<T, Container> 
			Type; 
	};

}