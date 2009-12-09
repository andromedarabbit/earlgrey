#pragma once

#include "StlCustomAllocator.hpp"

namespace Earlgrey {
	struct GreedyDeleter
	{
		static void Delete(void* p)
		{
			gMemoryAllocator::Instance().Free( p );
		}
	};

	template<typename T, class Deleter>
	struct StlPrimitiveTypeDeleter
	{
		void operator()(T* p)
		{
			Deleter::Delete( p );
		}
	};

	template<typename T, class Deleter>
	struct StlObjectTypeDeleter
	{
		void operator()(T* p)
		{
			p->~T();
			Deleter::Delete( p );
		}
	};

	//! shared_ptr 에서 pointer의 deleter로 사용된다.
	template<typename T, class Deleter>
	struct StlCustomDeleter
	{
		typedef typename mpl::if_<
			std::numeric_limits<T>::is_specialized, 
			StlPrimitiveTypeDeleter<T,Deleter>, 
			StlObjectTypeDeleter<T,Deleter>
		>::type Type;
	};

	template<typename T, class Deleter>
	struct StlCustomDeleter<T*, Deleter>
	{
		typedef StlPrimitiveTypeDeleter<T*,Deleter> Type;
	};



	template<typename T>
	std::tr1::shared_ptr<T> make_ptr(T* p)
	{
		// 세번째 파라미터인 custom memory manager는 counter의 alloc/dealloc에 사용된다.
		// counter alloc/dealloc에 사용되는 custom memory manager는 counter type으로 rebind되서 사용되므로
		// template 인 T 가 어떤 타입이라도 상관 없다.
		return std::tr1::shared_ptr<T>( 
			p, 
			StlCustomDeleter<T, GreedyDeleter>::Type(), 
			StlDefaultAllocator<T>::Type() );
	}

	template<typename T>
	void reset_ptr(std::tr1::shared_ptr<T>& ptr, T* p)
	{
		ptr.reset( 
			p, 
			StlCustomDeleter<T, GreedyDeleter>::Type(), 
			StlDefaultAllocator<T>::Type() );
	}
}