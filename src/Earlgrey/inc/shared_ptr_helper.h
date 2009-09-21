#pragma once

#include "StlCustomAllocator.hpp"

namespace Earlgrey {

	//! \todo memory manager ������ ���� deletor�� ���� ����� ��� ��.

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

	//! shared_ptr ���� pointer�� deletor�� ���ȴ�.
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
		// ����° �Ķ������ custom memory manager�� counter�� alloc/dealloc�� ���ȴ�.
		// counter alloc/dealloc�� ���Ǵ� custom memory manager�� counter type���� rebind�Ǽ� ���ǹǷ�
		// template �� T �� � Ÿ���̶� ��� ����.
		return std::tr1::shared_ptr<T>( p, StlCustomDeletor<T>::Type(), StlDefaultAllocator<T>::Type() );
	}
}