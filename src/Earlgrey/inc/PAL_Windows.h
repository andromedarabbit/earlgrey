#pragma  once

/*
// Windows Platform Adaptive Layer
#include <winsock2.h>

#define NOMINMAX // min, max 매크로 없애기
#include <windows.h>
*/

// Atomic Operator
#define AtomicInc(a)									InterlockedIncrement((volatile long *)(&(a)))
#define AtomicDec(a)									InterlockedDecrement((volatile long *)(&(a)))
#ifdef _WIN64
#define AtomicExch(Dest, Value)							InterlockedExchangePointer((volatile PVOID *)(&(Dest)), (PVOID)(Value))
#else
#define AtomicExch(Dest, Value)							InterlockedExchange((volatile long *)(&(Dest)), (long)(Value))
#endif
