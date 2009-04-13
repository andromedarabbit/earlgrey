#pragma once
#include <Windows.h>
//#include <process.h>
//#include <crtdbg.h>

inline BOOL CAS(volatile LONG *Dest, LONG Compare, LONG Exchange)
{
	return InterlockedCompareExchange( Dest, Exchange, Compare ) == static_cast<LONG>(Compare);
}

inline BOOL CAS64(volatile LONGLONG *Dest, LONGLONG Compare, LONGLONG Exchange)
{   
#ifdef _WIN64
	return InterlockedCompareExchange64((volatile LONGLONG*)Dest, Exchange, Compare) == static_cast<LONGLONG>(Compare);
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
		//    Compare (EDX:EAX) with operand, if equal (ECX:EBX) is stored
		lock cmpxchg8b qword ptr [edi]
		pop edi
		pop edx
		pop eax
		pop ebx
		pop ecx
		je    success
		mov dword ptr [Return], 0       
success:
	}
	return Return;
#endif
}


template<typename T>
class LockfreeStack
{
public:
	struct Cell
	{
		struct Cell*    next;
		T                value;
	};

private:
	union LIFO
	{
		struct
		{
			struct Cell*    p;
			size_t            count;
		} val;

		LONGLONG val64;
	};



public:
	LockfreeStack()
	{
		_Top.val64 = 0L;
	}

	~LockfreeStack()
	{
	}

	void Push(T value)
	{
		struct Cell* cell = new struct Cell;
		cell->value = value;
		cell->next = 0;

		for (;;)
		{
			cell->next = _Top.val.p;
			if (CAS( (volatile LONG*)&_Top.val.p, (LONG)cell->next, (LONG)cell ))
			{
				break;
			}
		}
	}

	T Pop()
	{
		union LIFO head;

		for (;;)
		{
			head.val64 = _Top.val64;

			if (!head.val.p)
				return NULL;

			union LIFO next;
			next.val.p = head.val.p->next;
			next.val.count = head.val.count + 1;

			if (CAS64( (volatile LONGLONG*)&_Top.val64, head.val64, next.val64 ))
			{
				break;
			}
		}

		//_ASSERT(_Top.val.count < 10000000);

		T ret = head.val.p->value;
		delete head.val.p;

		return ret;
	}

private:
	volatile union LIFO _Top;
};
