#pragma once
#include "lockfree.h"

namespace Earlgrey { namespace Algorithm { namespace Lockfree {
	
	template<typename T>
	class Queue : private Uncopyable
	{
	public:
		typedef union Pointer<T> PointerType;
		typedef struct Cell<T> CellType;

	private:
		PointerType _head;
		PointerType _tail;

	public:
		explicit Queue()
		{
			_head.p( new CellType() );
			_head.Count( 0 );
			_tail = _head;
		}

		void Enqueue(T value)
		{
			PointerType tail, next, newCell;
			newCell.p( new CellType( value ) );

			for(;;)
			{
				tail = _tail;
				next.p( tail.p()->next );

				if (tail.val64 != _tail.val64)
					continue;

				if (NULL == next.p())
				{
#ifdef _WIN64
					if (CAS64( (volatile LONGLONG*) &(_tail.p()->next), (LONGLONG) next.p(), (LONGLONG) newCell.p() ))
#else
					if (CAS( (volatile LONG*) &(_tail.p()->next), (LONG) next.p(), (LONG) newCell.p() ))
#endif
					{
						newCell.Count( tail.Count() + 1 );
						CAS64( (volatile LONGLONG*) &_tail.val64, tail.val64, newCell.val64 );
						break;
					}
				}
				else
				{
					// If _tail.next has been changed but _tail has not been changed, 
					// change _tail into next-cell which is a new cell
					next.Count( tail.Count() + 1 );
					CAS64( (volatile LONGLONG*) &_tail.val64, tail.val64, next.val64 );
				}
			}
		}

		bool Dequeue(T& value)
		{
			for (;;)
			{
				PointerType head = _head, tail = _tail;
				PointerType next;
				next.p( head.p()->next ); // next cell of head is real.

				if (head.val64 != _head.val64)
					continue;

				// is empty?
				if (head.val64 == tail.val64)
					return false;

				value = next.p()->value;

				next.Count( head.Count() + 1 );
				if (CAS64( (volatile LONGLONG*) &_head.val64, head.val64, next.val64 ))
				{
					delete head.p();
					return true;
				}
			}
		}
	};

}}}