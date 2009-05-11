#pragma once
#include <Windows.h>
#include "Uncopyable.h"

namespace Earlgrey {

	namespace Algorithm {

		inline BOOL CAS(volatile LONG *Dest, LONG Compare, LONG Exchange)
		{   
			return InterlockedCompareExchange(Dest, Exchange, Compare) == Compare;
		}

		inline BOOL CAS64(volatile LONGLONG *Dest, LONGLONG Compare, LONGLONG Exchange)
		{   
			return InterlockedCompareExchange64(Dest, Exchange, Compare) == Compare;
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
			// So, we use high 20 bits as a counter;
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

				ULONG Count() const
				{
					return val.count;
				}

				void Count(ULONG count)
				{
					val.count = count;
				}
		#endif
			};

		} // end of Lockfree namespace
	} // end of Thread namespace
}