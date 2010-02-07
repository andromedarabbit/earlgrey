#pragma once

#include "Uncopyable.h"

#include "MemoryMath.h"
#include "MemoryBlock.h"
#include "SlabAllocator.h"
#include "GreedyAllocator.h"
#include "ThreadLocalAllocator.h"

#include <Loki/Singleton.h>
#include <memory>


namespace Earlgrey
{
	class DefaultMemoryAllocator
	{
	public:
		typedef size_t size_type;

		void* Alloc(size_type bytes)
		{
			return malloc(bytes);
		}

		inline void Free(void * ptr)
		{
			free(ptr);
		}
	};

#define USE_MEMORY_ALLOCATOR_THREAD_LOCAL_SLAB 1

#ifndef USE_MEMORY_ALLOCATOR_GREEDY
#define USE_MEMORY_ALLOCATOR_GREEDY 0
#endif

#ifndef USE_MEMORY_ALLOCATOR_THREAD_LOCAL_SLAB
#define USE_MEMORY_ALLOCATOR_THREAD_LOCAL_SLAB 0
#endif

#ifndef USE_MEMORY_ALLOCATOR_LOW_FRAGMENTATION_HEAP
#define USE_MEMORY_ALLOCATOR_LOW_FRAGMENTATION_HEAP 0
#endif

#ifndef USE_MEMORY_ALLOCATOR_THREAD_BUILDING_BLOCK
#define USE_MEMORY_ALLOCATOR_THREAD_BUILDING_BLOCK 0
#endif

#define DEFINE_GLOBAL_MEMORY_ALLOCATOR(AllocatorName) \
	typedef Loki::SingletonHolder<AllocatorName, Loki::CreateUsingNew, Loki::DefaultLifetime, Loki::SingleThreaded> gMemoryAllocator;

#if USE_MEMORY_ALLOCATOR_GREEDY
	DEFINE_GLOBAL_MEMORY_ALLOCATOR(GreedyAllocator)
#elif USE_MEMORY_ALLOCATOR_THREAD_LOCAL_SLAB
	DEFINE_GLOBAL_MEMORY_ALLOCATOR(ThreadLocalAllocator<SlabAllocator>)
#elif USE_MEMORY_ALLOCATOR_LOW_FRAGMENTATION_HEAP
	DEFINE_GLOBAL_MEMORY_ALLOCATOR(LowFragmentationHeapMemoryAllocator)
#elif USE_MEMORY_ALLOCATOR_THREAD_BUILDING_BLOCK
	DEFINE_GLOBAL_MEMORY_ALLOCATOR(ThreadBuildingBlockMemoryAllocator)
#else
	DEFINE_GLOBAL_MEMORY_ALLOCATOR(DefaultMemoryAllocator)
#endif
}