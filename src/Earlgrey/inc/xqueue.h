#pragma once 
#include "StlCustomAllocator.hpp"

#include <queue>


namespace Earlgrey
{

	template<typename T, typename Container = xdeque<T>::Type >
	struct xqueue
	{
		typedef 
			std::queue<T, Container> 
			Type; 
	};

	template<typename T, typename Container = xdeque<T>::Type >
	struct xpriority_queue
	{
		typedef 
			std::priority_queue<T, Container> 
			Type; 
	};


}