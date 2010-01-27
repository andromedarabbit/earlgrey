#pragma once
#define NOMINMAX
#include <Windows.h>

#include "EarlgreyAssert.h"
#include "Uncopyable.h"

namespace Earlgrey { namespace Algorithm { 

	inline BOOL CAS(volatile LONG *Dest, LONG Compare, LONG Exchange)
	{   
		return InterlockedCompareExchange(Dest, Exchange, Compare) == Compare;
	}

	inline BOOL CAS64(volatile LONGLONG *Dest, LONGLONG Compare, LONGLONG Exchange)
	{   
		return InterlockedCompareExchange64(Dest, Exchange, Compare) == Compare;
#if 0
#ifdef _WIN64
		return InterlockedCompareExchange64(Dest, Exchange, Compare) == Compare;
#else
		BOOL Return = TRUE;
		__asm
		{		
			push ecx
			push ebx
			push eax
			push edx
			push edi

			mov edx, dword ptr [Compare] + 4
			mov eax, dword ptr [Compare]
			mov ecx, dword ptr [Exchange] + 4
			mov ebx, dword ptr [Exchange]
			mov edi, Dest
			//	Compare (EDX:EAX) with operand, if equal (ECX:EBX) is stored
			lock cmpxchg8b qword ptr [edi]
			pop edi
			pop edx
			pop eax
			pop ebx
			pop ecx
			je	success
			mov dword ptr [Return], 0		
success:
		}
		return Return;
#endif
#endif
	}
}}

namespace Earlgrey { namespace Algorithm { namespace Lockfree {



	// http://msdn.microsoft.com/en-us/library/aa366778.aspx
	// assume that user-mode virtual address space is 7TB (using just low 42 bits),
	// we won't use high 20 bits (more precisely 22 bits).
	// So, we use high 20 bits as a counter;
	static const ULONGLONG maskOfCounter64 = 0xfffff00000000000;
	static const ULONGLONG maskOfPointer64 = 0x00000fffffffffff;


	template<typename T>
	union Pointer
	{
		explicit Pointer() {}

		explicit Pointer(typename T* pointer, ULONG count) {

			p(pointer);
			Count(count);
		}

		explicit Pointer(typename T* pointer) {
			p(pointer);
			Count(Count() + 1);

		}
		volatile ULONGLONG val64;

#ifdef _WIN64
		struct {
			ULONGLONG p : 44;		// low 44 bits for pointer
			ULONGLONG count : 20;	// high 20 bits for counter
		} val;

		inline typename T* p() const
		{
			return reinterpret_cast<T*>( val.p );
		}

		inline void p(typename T* pointer)
		{
			EARLGREY_ASSERT((reinterpret_cast<ULONGLONG>(pointer) & maskOfCounter64) == 0);
			val.p = reinterpret_cast<ULONGLONG>(pointer);
		}

		inline ULONG Count() const
		{
			return (ULONG)val.count;
		}

		inline void Count(ULONG count)
		{
			val.count = count;
		}
#else
		struct {
			T* p;
			ULONG count;
		} val;

		inline typename T* p() const
		{
			return (T*)val.p;
		}

		inline void p(typename T* pointer)
		{
			val.p = pointer;
		}

		inline ULONG Count() const
		{
			return (ULONG)val.count;
		}

		inline void Count(ULONG count)
		{
			val.count = count;
		}
#endif
		inline ULONG NextCount() const
		{
			return Count() + 1;
		}
	};


	template<class T>
	inline bool operator==(const Pointer<T>& A, const Pointer<T>& B)
	{
		return A.val64 == B.val64;
	}

	template<class T>
	inline bool operator!=(const Pointer<T>& A, const Pointer<T>& B)
	{
		return A.val64 != B.val64;
	}



	template<typename T>
	struct Cell
	{
		typedef struct Cell<T> CellType;

		typedef union Pointer<CellType> PointerType;
		
		explicit Cell(CellType* next, T value)
		{
			this->next.p(next);
			this->value = value;
		}

		explicit Cell(T value)
		{
			this->next.p(NULL);
			this->value = value;
		}

		explicit Cell() { this->next.p(NULL); }

		PointerType		next;
		T				value;
		
	};

}}}