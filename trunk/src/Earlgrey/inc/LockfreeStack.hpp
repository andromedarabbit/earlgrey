#pragma once
#include "lockfree.h"

namespace Earlgrey { namespace Algorithm { namespace Lockfree {

	template<typename T>
	class Stack : private Uncopyable
	{
	public:
		typedef struct Cell<T> CellType;
		typedef union Pointer<T> PointerType;

	private:
		PointerType _head;

	public:
		explicit Stack()
		{
			_head.val64 = 0L;
		}

		~Stack()
		{
		}

		//! normal push operation of stack
		void Push(T value)
		{
			CellType* cell = new CellType( 0, value );
			PointerType head, newItem;
			newItem.p( cell );

			do {
				head = _head;
				newItem.p()->next = _head.p();
				newItem.Count( _head.Count() + 1 );
			} while(!CAS64( (volatile LONGLONG*)&_head.val64, (LONGLONG)head.val64, (LONGLONG)newItem.val64 ));
		}

		//! normal pop operation of stack
		/*!
			\param value is an output parameter, if pop() returns false, pop() won't set any value.
			\return if stack is empty, then return false, otherwise return true
		*/
		bool Pop(T& value)
		{
			PointerType head, next;
			head.val64 = _head.val64;

			while(head.val.p) {
				next.p( head.p()->next );
				next.Count( head.Count() + 1 );

				if (CAS64( (volatile LONGLONG*)&_head.val64, head.val64, next.val64 ))
				{
					value = head.p()->value;
					delete head.p();
					return true;
				}
				head.val64 = _head.val64;
			}
			return false;
		}
	};

}}}