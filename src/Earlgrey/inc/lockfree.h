#pragma once
#include <Windows.h>
#include "Uncopyable.h"

namespace Earlgrey {

	namespace Thread {

	inline BOOL CAS(volatile LONG *Dest, LONG Compare, LONG Exchange)
	{   
		return InterlockedCompareExchange((volatile LONG*)Dest, Exchange, Compare) == static_cast<LONG>(Compare);
	}

	inline BOOL CAS64(volatile LONGLONG *Dest, LONGLONG Compare, LONGLONG Exchange)
	{   
		return InterlockedCompareExchange64((volatile LONGLONG*)Dest, Exchange, Compare) == static_cast<LONGLONG>(Compare);
	}


	namespace Lockfree {

		template<typename T>
		struct Cell
		{
			Cell(struct Cell<T>* next, T value)
			{
				this->next = next;
				this->value = value;
			}

			Cell(T value)
			{
				next = NULL;
				this->value = value;
			}

			Cell() { next = NULL; }

			struct Cell<T>*	next;
			T				value;
		};

		// http://msdn.microsoft.com/en-us/library/aa366778.aspx
		// assume that user-mode virtual address space is 7TB (using just low 42 bits),
		// we won't use high 20 bits (more precisely 22 bits).
		// So, we use high 20 bits as the counter;
		static const ULONGLONG maskOfCounter64 = 0xfffff00000000000;
		static const ULONGLONG maskOfPointer64 = 0x00000fffffffffff;


		template<typename T>
		union Pointer
		{
			typedef struct Cell<T> CellType;

			volatile ULONGLONG val64;

	#ifdef _WIN64
			struct {
				ULONGLONG p : 44;		// low 44 bits for pointer
				ULONGLONG count : 20;	// high 20 bits for counter
			} val;

			CellType* p()
			{
				return reinterpret_cast<CellType*>( val.p );
			}

			void p(CellType* pointer)
			{
				_ASSERTE((reinterpret_cast<ULONGLONG>(pointer) & maskOfCounter64) == 0);
				val.p = reinterpret_cast<ULONGLONG>(pointer);
			}

			ULONG Count()
			{
				return (ULONG)val.count;
			}

			void Count(ULONG count)
			{
				val.count = count;
			}
	#else
			struct {
				CellType* p;
				ULONG count;
			} val;

			struct CellType* p()
			{
				return val.p;
			}

			void p(CellType* pointer)
			{
				val.p = pointer;
			}

			ULONG Count()
			{
				return val.count;
			}

			void Count(ULONG count)
			{
				val.count = count;
			}
	#endif
		};

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
	} // end of Lockfree namespace
	} // end of Thread namespace
}