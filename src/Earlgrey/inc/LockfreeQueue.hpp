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
			// �ٸ� �����尡 "�� ť"�� ���� ť�������� ��ó tail�� ������Ʈ���� ���� ���,
			// ť�� ���� �������� �ұ��ϰ� Dequeue() �� false�� �����Ѵ�.
			// �׷��� ������ Dequeue()�� ȣ���ϱ� ���� tail�� ���� ��ġ�� �Ű���� �Ѵ�.
			for (;;)
			{
				PointerType head = _head, tail = _tail;
				PointerType next = tail.p()->next;

				// head is updated? wait..
				if (head.val64 != _head.val64) continue;

				// ť�� ������� ������ tail�� �ű��� �ʾƵ� �ȴ�.
				if (head.p() != tail.p())  return;

				// ť�� �����Ͱ� �ְ� tail�� next�� null�̸� ����!! �ű� �ʿ䰡 ����.
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

				// Dequeue() ȣ�� ���� MoveTail() �� ȣ������ ������ ť�� �� ������ ó���Ǿ�
				// false�� ������ ���� �ִ�.
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