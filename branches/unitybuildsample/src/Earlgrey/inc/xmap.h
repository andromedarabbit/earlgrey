#pragma once 
#include "StlCustomAllocator.hpp"

#include <map>


namespace Earlgrey
{
	template<typename Key, typename T, typename Compare = std::less<Key> >
	struct xmap
	{ 
		typedef 
			std::map<Key, T, Compare,  typename StlDefaultAllocator< std::pair<Key, T> >::Type  > 
			Type; 
	};

	template<typename Key, typename T, typename Compare = std::less<Key> >
	struct auto_map
	{ 
		typedef 
			std::map<Key, T, Compare,  typename StlStackAllocator< std::pair<Key, T> >  > 
			Type; 
	};

	template<typename Key, typename T, typename Compare = std::less<Key> >
	struct xmultimap
	{
		typedef 
			std::multimap<Key, T, Compare,  typename StlDefaultAllocator< std::pair<Key, T> >::Type > 
			Type; 
	};


	template<typename Key, typename T, typename Compare = std::less<Key> >
	struct auto_multimap
	{
		typedef 
			std::multimap<Key, T, Compare,  typename StlStackAllocator< std::pair<Key, T> > > 
			Type; 
	};
}