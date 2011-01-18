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

	//! shared_ptr ���� pointer�� deleter�� ���ȴ�.
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
		// ����° �Ķ������ custom memory manager�� counter�� alloc/dealloc�� ���ȴ�.
		// counter alloc/dealloc�� ���Ǵ� custom memory manager�� counter type���� rebind�Ǽ� ���ǹǷ�
		// template �� T �� � Ÿ���̶� ��� ����.
		// �ι�°�� ����°�� ���� �޸� �Ҵ��ڿ��� �Ѵ�. ī���͸� �Ҵ��� �� ����° �Ҵ��ڸ� �̿�������
		// ī���͸� ������ �� �ι��� �����ڸ� �̿��ϱ� �����̴�.
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

