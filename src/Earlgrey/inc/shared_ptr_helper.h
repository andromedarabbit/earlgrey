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

	//! shared_ptr ���� pointer�� deleter�� ���ȴ�.
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
		// ����° �Ķ������ custom memory manager�� counter�� alloc/dealloc�� ���ȴ�.
		// counter alloc/dealloc�� ���Ǵ� custom memory manager�� counter type���� rebind�Ǽ� ���ǹǷ�
		// template �� T �� � Ÿ���̶� ��� ����.
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