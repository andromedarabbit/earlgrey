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
		typedef size_t size_type;

		void* Alloc(size_type bytes)
		{
			return malloc(bytes);
		}

		inline void Free(void * ptr)
		{
			free(ptr);
		}

		template <class T> 
		struct CreateUsingNew
		{
			static T* Create()
			{ 
				return new T();
			}

			static void Destroy(T* p)
			{ 
				delete p; 
			}
		};
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

#if USE_MEMORY_ALLOCATOR_GREEDY
	typedef Loki::SingletonHolder<GreedyAllocator, GreedyAllocator::CreateUsingNew, Loki::DefaultLifetime, Loki::SingleThreaded> 
		gMemoryAllocator;
#else if USE_MEMORY_ALLOCATOR_THREAD_LOCAL_SLAB
	typedef Loki::SingletonHolder<ThreadLocalAllocator<SlabAllocator>, ThreadLocalAllocator<SlabAllocator>::CreateUsingNew, Loki::DefaultLifetime, Loki::SingleThreaded> 
		gMemoryAllocator;
#else if USE_MEMORY_ALLOCATOR_LOW_FRAGMENTATION_HEAP
#else if USE_MEMORY_ALLOCATOR_THREAD_BUILDING_BLOCK
#else
	typedef Loki::SingletonHolder<DefaultMemoryAllocator, DefaultMemoryAllocator::CreateUsingNew, Loki::DefaultLifetime, Loki::SingleThreaded> 
		gMemoryAllocator;
#endif
}