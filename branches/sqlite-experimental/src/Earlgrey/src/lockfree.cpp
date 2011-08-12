#include "stdafx.h"
#include "lockfree.h"

namespace Earlgrey
{
	/*
	namespace Algorithm
	{
#ifdef _WIN64
#else
		BOOL CAS64(volatile LONGLONG *Dest, LONGLONG Compare, LONGLONG Exchange)
		{   
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
		}
#endif
	}
	*/
}