#pragma once
#include "MemoryAllocator.h"

namespace Earlgrey
{
	// TEMPLATE CLASS StlGreedyAllocator
	template<class _Ty>
	class StlGreedyAllocator
		: public std::_Allocator_base<_Ty>
	{	// generic allocator for objects of class _Ty
	public:
		typedef std::_Allocator_base<_Ty> _Mybase;
		typedef typename _Mybase::value_type value_type;
		typedef value_type _FARQ *pointer;
		typedef value_type _FARQ& reference;
		typedef const value_type _FARQ *const_pointer;
		typedef const value_type _FARQ& const_reference;

		typedef _SIZT size_type;
		typedef _PDFT difference_type;

		struct Deleter
		{
			//static void Delete(_Ty* p)
			//{
			//	StlGreedyAllocator allocator;
			//	allocator.deallocate( p, sizeof(_Ty) );
			//}

			inline
				void operator()(_Ty* p)
			{
				StlGreedyAllocator allocator;
				allocator.deallocate( p, sizeof(_Ty) );
			}
		};

		template<class _Other>
		struct rebind
		{	// convert an allocator<_Ty> to an allocator <_Other>
			typedef StlGreedyAllocator<_Other> other;
		};

		pointer address(reference _Val) const
		{	// return address of mutable _Val
			return (&_Val);
		}

		const_pointer address(const_reference _Val) const
		{	// return address of nonmutable _Val
			return (&_Val);
		}

		StlGreedyAllocator() _THROW0()
		{	// construct default allocator (do nothing)
		}

		StlGreedyAllocator(const StlGreedyAllocator<_Ty>&) _THROW0()
		{	// construct by copying (do nothing)
		}

		template<class _Other>
		StlGreedyAllocator(const StlGreedyAllocator<_Other>&) _THROW0()
		{	// construct from a related allocator (do nothing)
		}

		template<class _Other>
		StlGreedyAllocator<_Ty>& operator=(const StlGreedyAllocator<_Other>&)
		{	// assign from a related allocator (do nothing)
			return (*this);
		}

		void deallocate(pointer _Ptr, size_type)
		{	// deallocate object at _Ptr, ignore size
			// ::operator delete(_Ptr);
			gMemoryAllocator::Instance().Free(_Ptr);
		}

		pointer allocate(size_type _Count)
		{	// allocate array of _Count elements
			// return (std::_Allocate(_Count, (pointer)0));
			return static_cast<pointer>( 
				gMemoryAllocator::Instance().Alloc(_Count * sizeof(_Ty))
				);
		}

		pointer allocate(size_type _Count, const void _FARQ *)
		{	// allocate array of _Count elements, ignore hint
			return (allocate(_Count));
		}

		void construct(pointer _Ptr, const _Ty& _Val)
		{	// construct object at _Ptr with value _Val
			std::_Construct(_Ptr, _Val);
		}

		void destroy(pointer _Ptr)
		{	// destroy object at _Ptr
			std::_Destroy(_Ptr);
		}

		_SIZT max_size() const _THROW0()
		{	// estimate maximum array size
			_SIZT _Count = (_SIZT)(-1) / sizeof (_Ty);
			return (0 < _Count ? _Count : 1);
		}
	};

	// allocator TEMPLATE OPERATORS
	template<class _Ty,
	class _Other> inline
		bool operator==(const StlGreedyAllocator<_Ty>&, const StlGreedyAllocator<_Other>&) _THROW0()
	{	// test for allocator equality (always true)
		return (true);
	}

	template<class _Ty,
	class _Other> inline
		bool operator!=(const StlGreedyAllocator<_Ty>&, const StlGreedyAllocator<_Other>&) _THROW0()
	{	// test for allocator inequality (always false)
		return (false);
	}

	// CLASS allocator<void>
	template<> class _CRTIMP2_PURE StlGreedyAllocator<void>
	{	// generic allocator for type void
	public:
		typedef void _Ty;
		typedef _Ty _FARQ *pointer;
		typedef const _Ty _FARQ *const_pointer;
		typedef _Ty value_type;

		template<class _Other>
		struct rebind
		{	// convert an allocator<void> to an allocator <_Other>
			typedef StlGreedyAllocator<_Other> other;
		};

		StlGreedyAllocator() _THROW0()
		{	// construct default allocator (do nothing)
		}

		StlGreedyAllocator(const StlGreedyAllocator<_Ty>&) _THROW0()
		{	// construct by copying (do nothing)
		}

		template<class _Other>
		StlGreedyAllocator(const StlGreedyAllocator<_Other>&) _THROW0()
		{	// construct from related allocator (do nothing)
		}

		template<class _Other>
		StlGreedyAllocator<_Ty>& operator=(const StlGreedyAllocator<_Other>&)
		{	// assign from a related allocator (do nothing)
			return (*this);
		}
	};

}