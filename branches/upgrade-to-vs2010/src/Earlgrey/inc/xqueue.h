#pragma once 
#include "StlCustomAllocator.hpp"

#include "xdeque.h"
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

	template<typename T, typename Container = auto_deque<T>::Type >
	struct auto_queue
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

	template<typename T, typename Container = auto_deque<T>::Type >
	struct auto_priority_queue
	{
		typedef 
			std::priority_queue<T, Container> 
			Type; 
	};


}