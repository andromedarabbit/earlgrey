#pragma once
#include "lockfree.h"

namespace Earlgrey { namespace Algorithm { namespace Lockfree {
	
	template<typename T>
	class Queue : private Uncopyable
	{
	private:
		
		typedef struct Cell<T> CellType;
		typedef union Pointer<CellType> PointerType;

	private:
		PointerType _head;
		PointerType _tail;

	public:
		Queue()
		{
			_head.p( new CellType() );
			_tail = _head;
		}

		void Enqueue(T value)
		{
			PointerType tail, next, newCell;
			newCell.p( new CellType( value ) );

			_ASSERTE(newCell.p()->next.p() == NULL);

			for(;;)
			{
				tail = _tail;
				next = tail.p()->next;

				if (tail.val64 != _tail.val64)
					continue;

				if (NULL == next.p())
				{
					newCell.Count(next.Count() + 1);
					if (CAS64( (volatile LONGLONG*) &(_tail.p()->next), (LONGLONG) next.val64, (LONGLONG) newCell.val64 ))
					{
						PointerType Ptr;
						Ptr.p(newCell.p());
						Ptr.Count(tail.Count() + 1);
						CAS64( (volatile LONGLONG*) &_tail.val64, tail.val64, Ptr.val64 );
						break;
					}
				}
				else
				{
					// If _tail.next has been changed but _tail has not been changed, 
					// change _tail into next-cell which is a new cell
					next.Count( tail.Count() + 1 );
					_ASSERTE(next.p() == NULL);
					CAS64( (volatile LONGLONG*) &_tail.val64, tail.val64, next.val64);
				}
			}
		}

		void MoveTail() 
		{
			for (;;)
			{
				PointerType head = _head, tail = _tail;
				PointerType next;
				next = head.p()->next; // next cell of head is real.

				if (head.val64 != _head.val64)
					continue;

				if (next.p() == NULL) continue;

				if (head.val64 != tail.val64)  return;


				next.Count(tail.Count() + 1);

				CAS64( (volatile LONGLONG*) &_tail.val64, tail.val64, next.val64);

				_ASSERTE(_head.val64 != _tail.val64);
			}

		}

		bool Dequeue(T& value)
		{
			for (;;)
			{
				PointerType head = _head, tail = _tail;
				PointerType next = head.p()->next;

				if (head.val64 != _head.val64)
					continue;

				if (head.val64 == tail.val64)  return false;

				_ASSERTE(next.p() != NULL);

				value = next.p()->value;

				next.Count( head.Count() + 1 );
				if (CAS64( (volatile LONGLONG*) &_head.val64, head.val64, next.val64 ))
				{
					//delete head.p(); // FIXME 바로지우면 안됨. 다른 thread 에서 next traverse 에 사용되고 있을 수 있음
					return true;
				}
			}
		}
	};

}}}