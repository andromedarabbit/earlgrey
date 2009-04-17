#pragma once
#include <Windows.h>

namespace Earlgrey {

	inline BOOL CAS64(volatile LONGLONG *Dest, LONGLONG Compare, LONGLONG Exchange)
	{   
		return InterlockedCompareExchange64((volatile LONGLONG*)Dest, Exchange, Compare) == static_cast<LONGLONG>(Compare);
	}

	template<typename T>
	class LockfreeStack : private Uncopyable
	{
	private:
		struct Cell
		{
			Cell(struct Cell* next, T value)
			{
				this->next = next;
				this->value = value;
			}

			struct Cell*	next;
			T				value;
		};

		// http://msdn.microsoft.com/en-us/library/aa366778.aspx
		// assume that user-mode virtual address space is 7TB (using just low 42 bits),
		// we won't use high 20 bits (more precisely 22 bits).
		// So, we use high 20 bits as the counter;
		static const ULONGLONG maskOfCounter64 = 0xfffff00000000000;
		static const ULONGLONG maskOfPointer64 = 0x00000fffffffffff;

		union LIFO
		{
			volatile ULONGLONG val64;

#ifdef _WIN64
			struct {
				ULONGLONG p : 44;		// low 44 bits for pointer
				ULONGLONG count : 20;	// high 20 bits for counter
			} val;

			struct Cell* p()
			{
				return reinterpret_cast<struct Cell*>( val.p );
			}

			void p(struct Cell* pointer)
			{
				_ASSERTE((reinterpret_cast<ULONGLONG>(pointer) & maskOfCounter64) == 0);
				val.p = reinterpret_cast<ULONGLONG>(pointer);
			}

			ULONG count()
			{
				return (ULONG)val.count;
			}

			void count(ULONG count)
			{
				val.count = count;
			}
#else
			struct {
				struct Cell* p;
				ULONG count;
			} val;

			struct Cell* p()
			{
				return val.p;
			}

			void p(struct Cell* pointer)
			{
				val.p = pointer;
			}

			ULONG count()
			{
				return val.count;
			}

			void count(ULONG count)
			{
				val.count = count;
			}
#endif
		};

	public:
		explicit LockfreeStack()
		{
			_head.val64 = 0L;
		}

		~LockfreeStack()
		{
		}

		//! normal push operation of stack
		void push(T value)
		{
			struct Cell* cell = new struct Cell( 0, value );
			union LIFO head, newItem;
			newItem.p( cell );

			do {
				head = _head;
				newItem.p()->next = _head.p();
				newItem.count( _head.count() + 1 );
			} while(!CAS64( (volatile LONGLONG*)&_head.val64, (LONGLONG)head.val64, (LONGLONG)newItem.val64 ));
		}

		//! normal pop operation of stack
		/*!
			\param value is an output parameter, if pop() returns false, pop() won't set any value.
			\return if stack is empty, then return false, otherwise return true
		*/
		bool pop(T& value)
		{
			union LIFO head, next;
			head.val64 = _head.val64;

			while(head.val.p) {
				next.p( head.p()->next );
				next.count( head.count() + 1 );
				
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

	private:
		union LIFO _head;
	};

}