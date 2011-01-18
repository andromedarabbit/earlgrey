#pragma once
#include "MemoryAllocator.h"
#include "StlCustomAllocator.hpp"

namespace Earlgrey {

	template<typename T>
	struct DefaultDeleter
	{
		static void Delete(T* p)
		{
			StlDefaultAllocator<T>::Type allocator;
			allocator.deallocate( p, 0 );
		}
	};

	template<typename T, template<class> class Deleter>
	struct StlPrimitiveTypeDeleter
	{
		void operator()(T* p)
		{
			Deleter<T>::Delete( p );
		}
	};

	template<typename T, template<class> class Deleter>
	struct StlObjectTypeDeleter
	{
		void operator()(T* p)
		{
			p->~T();
			Deleter<T>::Delete( p );
		}
	};

	//! shared_ptr 에서 pointer의 deleter로 사용된다.
	template<typename T, template<class> class Deleter>
	struct StlCustomDeleter
	{
		typedef typename mpl::if_<
			std::numeric_limits<T>::is_specialized, 
			StlPrimitiveTypeDeleter<T,Deleter>, 
			StlObjectTypeDeleter<T,Deleter>
		>::type Type;
	};

	template<typename T, template<class> class Deleter>
	struct StlCustomDeleter<T*, Deleter>
	{
		typedef StlPrimitiveTypeDeleter<T*,Deleter> Type;
	};

	template<typename T>
	inline void* alloc()
	{
		StlDefaultAllocator<T>::Type allocator;
		return allocator.allocate(1);
	}

	inline void free(void* p)
	{
		StlDefaultAllocator<char>::Type allocator;
		allocator.deallocate( reinterpret_cast<char*>(p), 0 );
	}

	template<typename T>
	std::tr1::shared_ptr<T> make_ptr(T* p)
	{
		// 세번째 파라미터인 custom memory manager는 counter의 alloc/dealloc에 사용된다.
		// counter alloc/dealloc에 사용되는 custom memory manager는 counter type으로 rebind되서 사용되므로
		// template 인 T 가 어떤 타입이라도 상관 없다.
		// 두번째와 세번째는 같은 메모리 할당자여야 한다. 카운터를 할당할 땐 세번째 할당자를 이용하지만
		// 카운터를 제거할 땐 두번제 제거자를 이용하기 때문이다.
		return std::tr1::shared_ptr<T>( 
			p, 
			StlCustomDeleter<T, DefaultDeleter>::Type(), 
			StlDefaultAllocator<T>::Type() );
	}

	template<typename T>
	void reset_ptr(std::tr1::shared_ptr<T>& ptr, T* p)
	{
		ptr.reset( 
			p, 
			StlCustomDeleter<T, DefaultDeleter>::Type(), 
			StlDefaultAllocator<T>::Type() );
	}
}

