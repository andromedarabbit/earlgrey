#pragma once
#include "lockfree.h"
#include "EarlgreyAssert.h"

namespace Earlgrey { namespace Algorithm { namespace Lockfree {
	
	template<typename T, class Allocator = std::allocator<T>>
	class Queue : private Uncopyable
	{
	private:
		typedef struct Cell<T>				CellType;
		typedef union Pointer<CellType>		PointerType;
		typedef PointerPool<T, Allocator>	PointerPoolType;

	private:
		PointerType _head;
		PointerType _tail;

		volatile LONG _moveTailCount;

		static std::tr1::shared_ptr<PointerPoolType> GetHazardPointerPool()
		{
			static ThreadLocalValue<std::tr1::shared_ptr<PointerPoolType>> tlshazardPointerPool;
			if (!tlshazardPointerPool.IsAllocated())
			{
				tlshazardPointerPool = std::tr1::shared_ptr<PointerPoolType>(new PointerPoolType());
			}
			return tlshazardPointerPool.Get();
		}

	public:
		explicit Queue() : _moveTailCount(0)
		{
			_head.p( GetHazardPointerPool()->Allocate() );
			_head.Count(_head.NextCount());
			_tail = _head;
			EARLGREY_ASSERT(_tail.Count() == _head.Count());
			EARLGREY_ASSERT(_tail.p() == _head.p());
			EARLGREY_STATIC_ASSERT(sizeof(PointerType) == sizeof(ULONGLONG));
		}

		void Enqueue(T value)
		{
			CellType* cell = GetHazardPointerPool()->Allocate();
			cell->value = value;
			EARLGREY_VERIFY(cell != NULL);

			for(;;)
			{
				PointerType tail = _tail;
				PointerType next = tail.p()->next;

				// if another thread update _tail, wait..
				if (tail != _tail)
					continue;

				// if not pushed yet...
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
			// 다른 스레드가 "빈 큐"에 셀을 큐잉했지만 미처 tail을 업데이트하지 못한 경우,
			// 큐에 셀이 있음에도 불구하고 Dequeue() 가 false를 리턴한다.
			// 그렇기 때문에 Dequeue()를 호출하기 전에 tail을 정상 위치로 옮겨줘야 한다.
			for (;;)
			{
				PointerType head = _head, tail = _tail;
				PointerType next = tail.p()->next;

				// head is updated? wait..
				if (head.val64 != _head.val64) continue;

				// 큐가 비어있지 않으면 tail을 옮기지 않아도 된다.
				if (head.p() != tail.p())  return;

				// 큐에 데이터가 있고 tail의 next가 null이면 정상!! 옮길 필요가 없다.
				if (next.p() == NULL) return;

				// an item has been pushed, so update tail
				CAS64( (volatile LONGLONG*) &_tail.val64, tail.val64, PointerType(next.p(), tail.NextCount()).val64);
				InterlockedIncrement( &_moveTailCount );
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

				// Dequeue() 호출 전에 MoveTail() 을 호출하지 않으면 큐가 빈 것으로 처리되어
				// false를 리턴할 수도 있다.
				if (head.p() == tail.p())  return false;

				EARLGREY_ASSERT(next.p() != NULL);

				if (next.p() == NULL) continue;

				value = next.p()->value;

				EARLGREY_ASSERT(head.p() != next.p());

				if (!CAS64( (volatile LONGLONG*) &_head.val64, head.val64, PointerType(next.p(), head.NextCount()).val64))
				{
					continue;
				}

				EARLGREY_ASSERT(head.p() != _head.p());
				EARLGREY_ASSERT(head.p() != tail.p());

				GetHazardPointerPool()->Release( head.p() );

				return true;
			}
		}
	};

}}}