#pragma once
// TODO : 값을 전부 검토해야 한다. Environment::ProcessorCacheLineSize() 참고!
/*
#if defined(_WIN64) || defined(_M_ALPHA)
#define MAX_NATURAL_ALIGNMENT sizeof(ULONGLONG)
#define MEMORY_ALLOCATION_ALIGNMENT 16
#else
#define MAX_NATURAL_ALIGNMENT sizeof(DWORD)
#define MEMORY_ALLOCATION_ALIGNMENT 8
#endif



#ifndef SYSTEM_CACHE_ALIGNMENT_SIZE
#if defined(_AMD64_) || defined(_X86_)
#define SYSTEM_CACHE_ALIGNMENT_SIZE 64
#else
#define SYSTEM_CACHE_ALIGNMENT_SIZE 128
#endif
#endif
*/

#define EARLGREY_DEFAULT_ALLOCATION_ALIGNMENT MEMORY_ALLOCATION_ALIGNMENT // (winnt.h) 16 bytes on 64bit windows, 8 bytes on 32bit windows
#define EARLGREY_DEFAULT_CACHELINE_ALIGNMENT SYSTEM_CACHE_ALIGNMENT_SIZE // 256 // 32bytes ; 일바적인 x64 cpu
// Xeon : L1 64bytes(512bit), L2 128bytes
#define EARLGREY_DEFAULT_PAGE_ALIGNMENT 4096
