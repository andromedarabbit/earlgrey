#pragma once
#include <Windows.h>

namespace Earlgrey {

	inline BOOL CAS(volatile LONG *Dest, LONG Compare, LONG Exchange)
	{
		return InterlockedCompareExchange( Dest, Exchange, Compare ) == static_cast<LONG>(Compare);
	}

	inline BOOL CAS64(volatile LONGLONG *Dest, LONGLONG Compare, LONGLONG Exchange)
	{   
	//#ifdef _WIN64
		return InterlockedCompareExchange64((volatile LONGLONG*)Dest, Exchange, Compare) == static_cast<LONGLONG>(Compare);
	//#else
	//	BOOL Return = TRUE;
	//	__asm
	//	{       
	//		push ecx
	//		push ebx
	//		push eax
	//		push edx
	//		push edi

	//		mov edx, dword ptr [Compare] + 4
	//		mov eax, dword ptr [Compare]
	//		mov ecx, dword ptr [Exchange] + 4
	//		mov ebx, dword ptr [Exchange]
	//		mov edi, Dest
	//		//    Compare (EDX:EAX) with operand, if equal (ECX:EBX) is stored
	//		lock cmpxchg8b qword ptr [edi]
	//		pop edi
	//		pop edx
	//		pop eax
	//		pop ebx
	//		pop ecx
	//		je    success
	//		mov dword ptr [Return], 0       
	//success:
	//	}
	//	return Return;
	//#endif
	}


	template<typename T>
	class LockfreeStack32
	{
	public:
		struct Cell
		{
			struct Cell*	next;
			T				value;
		};

	private:
		union LIFO
		{
			struct
			{
				struct Cell*	p;
				size_t			count;	//!< just pop count (not push count)
			} val;

			LONGLONG val64;
		};

	public:
		explicit LockfreeStack32()
		{
			_head.val64 = 0L;
		}

		~LockfreeStack32()
		{
		}

		//! normal push operation of stack
		void push(T value)
		{
			struct Cell* cell = new struct Cell;
			cell->value = value;
			cell->next = 0;

			// We don't have to increase the pop count.
			do {
				cell->next = _head.val.p;
			} while(!CAS( (volatile LONG*)&_head.val.p, (LONG)cell->next, (LONG)cell ));
		}

		//! normal pop operation of stack
		/*!
			\param value is an output parameter, if pop() return false, pop() won't set any value.
			\return if stack is empty, then return false, otherwise return true
		*/
		bool pop(T& value)
		{
			union LIFO head, next;
			head.val64 = _head.val64;

			while(head.val.p) {
				next.val.p = head.val.p->next;
				next.val.count = head.val.count + 1;
				
				if (CAS64( (volatile LONGLONG*)&_head.val64, head.val64, next.val64 ))
				{
					value = head.val.p->value;
					delete head.val.p;
					return true;
				}
				head.val64 = _head.val64;
			}
			return false;
		}

	private:
		volatile union LIFO _head;
	};

}