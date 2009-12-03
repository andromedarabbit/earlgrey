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
		explicit Queue()
		{
			_head.p( new CellType() );
			_head.Count(_head.NextCount());
			_tail = _head;
			EARLGREY_ASSERT(_tail.Count() == _head.Count());
			EARLGREY_ASSERT(_tail.p() == _head.p());
			EARLGREY_STATIC_ASSERT(sizeof(PointerType) == sizeof(ULONGLONG));
		}

		void Enqueue(T value)
		{
			CellType* cell = new CellType(value);
			EARLGREY_VERIFY(cell != NULL);

			for(;;)
			{
				PointerType tail = _tail;
				PointerType next = tail.p()->next;

				if (tail != _tail)
					continue;

				if (NULL == next.p())
				{
					if (CAS64( (volatile LONGLONG*) &(_tail.p()->next), next.val64, PointerType(cell, next.NextCount()).val64 ))
					{
						CAS64( (volatile LONGLONG*) &_tail.val64, tail.val64, PointerType(cell, tail.NextCount()).val64);
						break;
					}
				}
				else
				{
					// If _tail.next has been changed but _tail has not been changed, 
					// change _tail into next-cell which is a new cell
					EARLGREY_ASSERT(next.p() != NULL);
					CAS64( (volatile LONGLONG*) &_tail.val64, tail.val64, PointerType(next.p(), tail.NextCount()).val64);
				}
			}
		}

		void MoveTail() 
		{
			for (;;)
			{
				PointerType head = _head, tail = _tail;
				PointerType next = tail.p()->next;

				if (head.val64 != _head.val64) continue;

				if (head.p() != tail.p())  return;

				if (next.p() == NULL) continue;

				CAS64( (volatile LONGLONG*) &_tail.val64, tail.val64, PointerType(next.p(), tail.NextCount()).val64);
			}

		}

		bool Dequeue(T& value)
		{
			for (;;)
			{
				PointerType head = _head, tail = _tail;
				PointerType next = head.p()->next;

				if (head != _head)
					continue;

				if (head.p() == tail.p())  return false;

				EARLGREY_ASSERT(next.p() != NULL);

				if (next.p() == NULL) continue;

				value = next.p()->value;

				EARLGREY_ASSERT(head.p() != next.p());

				//_head.val64 = next.val64;
				EARLGREY_VERIFY(CAS64( (volatile LONGLONG*) &_head.val64, head.val64, PointerType(next.p(), head.NextCount()).val64));

				EARLGREY_ASSERT(head.p() != _head.p());
				EARLGREY_ASSERT(head.p() != tail.p());

				delete head.p();  // FIXME 바로지우면 안됨. 다른 thread 에서 next traverse 에 사용되고 있을 수 있음, memory manager 수정후 

				return true;
			}
		}
	};

}}}