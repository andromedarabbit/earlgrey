#pragma  once
// TODO: �� ��ũ�θ� ���� ���ܾ� �� ������ ������?

#define AtomicInc(a)									InterlockedIncrement((volatile long *)(&(a)))
#define AtomicDec(a)									InterlockedDecrement((volatile long *)(&(a)))

#ifdef _WIN64
#define AtomicExch(Dest, Value)							InterlockedExchangePointer((volatile PVOID *)(&(Dest)), (PVOID)(Value))
#else
#define AtomicExch(Dest, Value)							InterlockedExchange((volatile long *)(&(Dest)), (long)(Value))
#endif
