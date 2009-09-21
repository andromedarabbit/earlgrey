#pragma once

#include "StlCustomAllocator.hpp"

namespace Earlgrey {
	template<typename T>
	struct StlCustomDeletor
	{
		void operator()(T* p)
		{
			StlDefaultAllocator<T>::Type _Alloc;
			_Alloc.deallocate( p, sizeof(T) );
		}
	};

	template<typename T>
	std::tr1::shared_ptr<T> create_shared_ptr(T* p)
	{
		return std::tr1::shared_ptr<T>( p, StlCustomDeletor<T>(), StlDefaultAllocator<T>::Type() );
	}
}