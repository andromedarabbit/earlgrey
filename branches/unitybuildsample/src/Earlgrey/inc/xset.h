#pragma once 
#include "StlCustomAllocator.hpp"

#include <set>


namespace Earlgrey
{
	template<typename Key, typename Compare = std::less<Key> >
	struct x_set
	{ 
		typedef 
			std::set<Key, Compare, typename StlDefaultAllocator< Key >::Type  > 
			Type; 
	};

	template<typename Key, typename T, typename Compare = std::less<Key> >
	struct auto_set
	{ 
		typedef 
			std::map<Key, Compare, typename StlStackAllocator< Key >  > 
			Type; 
	};

	template<typename Key, typename T, typename Compare = std::less<Key> >
	struct x_multiset
	{
		typedef 
			std::multiset<Key, Compare, typename StlDefaultAllocator< Key >::Type > 
			Type; 
	};


	template<typename Key, typename T, typename Compare = std::less<Key> >
	struct auto_multiset
	{
		typedef 
			std::multiset<Key, Compare,  typename StlStackAllocator< Key > > 
			Type; 
	};
}