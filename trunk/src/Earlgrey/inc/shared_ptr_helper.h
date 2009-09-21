#pragma once

#include "StlCustomAllocator.hpp"

namespace Earlgrey {

	//! \todo memory manager 종류에 따라 deletor를 각각 만들어 줘야 함.

	template<typename T>
	struct StlPrimitiveTypeDeletor
	{
		void operator()(T* p)
		{
			StlDefaultAllocator<T>::Type _Alloc;
			_Alloc.deallocate( p, sizeof(T) );
		}
	};

	template<typename T>
	struct StlObjectTypeDeletor
	{
		void operator()(T* p)
		{
			p->~T();
			StlDefaultAllocator<T>::Type _Alloc;
			_Alloc.deallocate( p, sizeof(T) );
		}
	};

	//! shared_ptr 에서 pointer의 deletor로 사용된다.
	template<typename T>
	struct StlCustomDeletor
	{
		typedef typename mpl::if_<
			std::numeric_limits<T>::is_specialized, 
			StlPrimitiveTypeDeletor<T>, 
			StlObjectTypeDeletor<T>
		>::type Type;
	};

	template<typename T>
	struct StlCustomDeletor<T*>
	{
		typedef StlPrimitiveTypeDeletor<T*> Type;
	};



	template<typename T>
	std::tr1::shared_ptr<T> create_shared_ptr(T* p)
	{
		// 세번째 파라미터인 custom memory manager는 counter의 alloc/dealloc에 사용된다.
		// counter alloc/dealloc에 사용되는 custom memory manager는 counter type으로 rebind되서 사용되므로
		// template 인 T 가 어떤 타입이라도 상관 없다.
		return std::tr1::shared_ptr<T>( p, StlCustomDeletor<T>::Type(), StlDefaultAllocator<T>::Type() );
	}
}