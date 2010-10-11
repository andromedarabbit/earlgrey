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

	template<typename T, typename Container = auto_deque<T>::Type >
	struct auto_stack
	{
		typedef 
			std::stack<T, Container> 
			Type; 
	};

}