#pragma  once

// Windows Platform Adaptive Layer
#include <winsock2.h>

#define NOMINMAX // min, max 매크로 없애기
#include <windows.h>


// Atomic Operator
#define AtomicInc(a)									InterlockedIncrement((volatile long *)(&(a)))
#define AtomicDec(a)									InterlockedDecrement((volatile long *)(&(a)))
#ifdef _WIN64
#define AtomicExch(Dest, Value)							InterlockedExchangePointer((volatile PVOID *)(&(Dest)), (PVOID)(Value))
#else
#define AtomicExch(Dest, Value)							InterlockedExchange((volatile long *)(&(Dest)), (long)(Value))
#endif

inline BOOL AtomicCAS(volatile LONG *Dest, LONG Compare, LONG Exchange)
{
	return InterlockedCompareExchange( Dest, Exchange, Compare ) == static_cast<LONG>(Compare);
}

inline BOOL AtomicCAS64(volatile LONGLONG *Dest, LONGLONG Compare, LONGLONG Exchange)
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


typedef unsigned char		UInt8;		// 8-bit  unsigned.
typedef unsigned short		UInt16;		// 16-bit unsigned.
typedef unsigned long		UInt32;		// 32-bit unsigned.
typedef unsigned __int64	UInt64;		// 64-bit unsigned.